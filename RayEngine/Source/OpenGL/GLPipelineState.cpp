#include "..\..\Include\OpenGL\GLDevice.h"
#include "..\..\Include\OpenGL\GLShader.h"
#include "..\..\Include\OpenGL\GLPipelineState.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLPipelineState::GLPipelineState(IDevice* pDevice, const PipelineStateDesc* pDesc)
			: m_Device(nullptr),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<GLDevice*>(pDevice);

			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLPipelineState::~GLPipelineState()
		{
			if (glIsProgram(m_Program))
			{
				glDeleteProgram(m_Program);
			}

			delete[] m_InputLayout.pElements;
			m_InputLayout.pElements = nullptr;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		PIPELINE_TYPE GLPipelineState::GetPipelineType() const
		{
			return m_Type;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLPipelineState::SetName(const std::string& name)
		{
			//Not relevant
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLPipelineState::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<GLDevice>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType GLPipelineState::GetReferenceCount() const
		{
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType GLPipelineState::Release()
		{
			IObject::CounterType refs = --m_References;
			if (refs < 1)
				delete this;

			return refs;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType GLPipelineState::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLPipelineState::Create(const PipelineStateDesc* pDesc)
		{
			if (pDesc->Type == PIPELINE_TYPE_GRAPHICS)
				CreateGraphicsPipeline(pDesc);
			else if (pDesc->Type == PIPELINE_TYPE_COMPUTE)
				CreateComputePipeline(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLPipelineState::CreateGraphicsPipeline(const PipelineStateDesc* pDesc)
		{
			m_Program = glCreateProgram();
			if (pDesc->GraphicsPipeline.pVertexShader != nullptr)
			{
				m_VS = pDesc->GraphicsPipeline.pVertexShader->QueryReference<GLShader>();
				glAttachShader(m_Program, m_VS->GetGLShaderID());
			}
			if (pDesc->GraphicsPipeline.pHullShader != nullptr)
			{
				m_HS = pDesc->GraphicsPipeline.pHullShader->QueryReference<GLShader>();
				glAttachShader(m_Program, m_HS->GetGLShaderID());
			}
			if (pDesc->GraphicsPipeline.pDomainShader != nullptr)
			{
				m_DS = pDesc->GraphicsPipeline.pDomainShader->QueryReference<GLShader>();
				glAttachShader(m_Program, m_DS->GetGLShaderID());
			}
			if (pDesc->GraphicsPipeline.pGeometryShader != nullptr)
			{
				m_GS = pDesc->GraphicsPipeline.pGeometryShader->QueryReference<GLShader>();
				glAttachShader(m_Program, m_GS->GetGLShaderID());
			}
			if (pDesc->GraphicsPipeline.pPixelShader != nullptr)
			{
				m_PS = pDesc->GraphicsPipeline.pPixelShader->QueryReference<GLShader>();
				glAttachShader(m_Program, m_PS->GetGLShaderID());
			}

			LinkShaders();
			CreateInputLayout(pDesc);
			CreateDepthState(pDesc);
			CreateRasterizerState(pDesc);
			CreateBlendState(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLPipelineState::CreateComputePipeline(const PipelineStateDesc* pDesc)
		{
			m_Program = glCreateProgram();
			if (pDesc->GraphicsPipeline.pGeometryShader != nullptr)
			{
				m_GS = pDesc->GraphicsPipeline.pGeometryShader->QueryReference<GLShader>();
				glAttachShader(m_Program, m_GS->GetGLShaderID());
			}

			LinkShaders();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLPipelineState::LinkShaders()
		{
			using namespace System;

			glLinkProgram(m_Program);

			int32 result = GL_TRUE;
			glGetProgramiv(m_Program, GL_LINK_STATUS, &result);
			if (result != GL_TRUE)
			{
				int32 len = 0;
				glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &len);

				std::vector<char> log;
				log.resize(len);
				glGetProgramInfoLog(m_Program, len, &len, log.data());

				std::string message = "OpenGL: Could not link program.\n";
				message += log.data();
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, message);
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLPipelineState::CreateInputLayout(const PipelineStateDesc* pDesc)
		{
			m_InputLayout.ElementCount = pDesc->GraphicsPipeline.InputLayout.ElementCount;
			m_InputLayout.pElements = new GLInputLayoutElement[m_InputLayout.ElementCount];

			InputElementDesc* pElements = pDesc->GraphicsPipeline.InputLayout.pElements;
			for (uint32 i = 0; i < m_InputLayout.ElementCount; i++)
			{
				m_InputLayout.pElements[i].Stride = pElements[i].StrideBytes;
				m_InputLayout.pElements[i].Offset = pElements[i].ElementOffset;
				m_InputLayout.pElements[i].Type = GetVertexFormat(pElements[i].Format);
				m_InputLayout.pElements[i].Size = GetVertexComponents(pElements[i].Format);
				m_InputLayout.pElements[i].Normalized = NormalizedVertexFormat(pElements[i].Format);
				m_InputLayout.pElements[i].Divisor = pElements[i].DataStepRate;
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLPipelineState::CreateDepthState(const PipelineStateDesc* pDesc)
		{
			m_DepthState.DepthEnable = pDesc->GraphicsPipeline.DepthStencilState.DepthEnable;
			m_DepthState.DepthFunc = ComparisonFuncToGL(pDesc->GraphicsPipeline.DepthStencilState.DepthFunc);
			
			if (pDesc->GraphicsPipeline.DepthStencilState.DepthWriteMask == DEPTH_WRITE_MASK_ALL)
				m_DepthState.DepthMask = GL_TRUE;
			else if (pDesc->GraphicsPipeline.DepthStencilState.DepthWriteMask == DEPTH_WRITE_MASK_ZERO)
				m_DepthState.DepthMask = GL_FALSE;
			else
				m_DepthState.DepthMask = 0;

			m_DepthState.StencilEnable = pDesc->GraphicsPipeline.DepthStencilState.StencilEnable;
			m_DepthState.WriteMask = pDesc->GraphicsPipeline.DepthStencilState.StencilWriteMask;

			m_DepthState.FrontFace.StencilFunc = ComparisonFuncToGL(pDesc->GraphicsPipeline.DepthStencilState.FrontFace.StencilFunc);
			m_DepthState.FrontFace.ReadMask = pDesc->GraphicsPipeline.DepthStencilState.StencilReadMask;
			m_DepthState.FrontFace.StencilFailOp = StencilOpToGL(pDesc->GraphicsPipeline.DepthStencilState.FrontFace.StencilFailOperation);
			m_DepthState.FrontFace.DepthFailOp = StencilOpToGL(pDesc->GraphicsPipeline.DepthStencilState.FrontFace.StencilDepthFailOperation);
			m_DepthState.FrontFace.PassOp = StencilOpToGL(pDesc->GraphicsPipeline.DepthStencilState.FrontFace.StencilPassOperation);

			m_DepthState.BackFace.StencilFunc = ComparisonFuncToGL(pDesc->GraphicsPipeline.DepthStencilState.BackFace.StencilFunc);
			m_DepthState.BackFace.ReadMask = pDesc->GraphicsPipeline.DepthStencilState.StencilReadMask;
			m_DepthState.BackFace.StencilFailOp = StencilOpToGL(pDesc->GraphicsPipeline.DepthStencilState.BackFace.StencilFailOperation);
			m_DepthState.BackFace.DepthFailOp = StencilOpToGL(pDesc->GraphicsPipeline.DepthStencilState.BackFace.StencilDepthFailOperation);
			m_DepthState.BackFace.PassOp = StencilOpToGL(pDesc->GraphicsPipeline.DepthStencilState.BackFace.StencilPassOperation);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLPipelineState::CreateRasterizerState(const PipelineStateDesc* pDesc)
		{
			m_RasterizerState.ConservativeRasterizerEnable = pDesc->GraphicsPipeline.RasterizerState.ConservativeRasterizerEnable;
			
			if (pDesc->GraphicsPipeline.RasterizerState.FillMode == FILL_MODE_SOLID)
				m_RasterizerState.PolygonMode = GL_FILL;
			else if (pDesc->GraphicsPipeline.RasterizerState.FillMode == FILL_MODE_WIREFRAME)
				m_RasterizerState.PolygonMode = GL_LINE;
			
			if (pDesc->GraphicsPipeline.RasterizerState.CullMode == CULL_MODE_BACK)
				m_RasterizerState.CullMode = GL_BACK;
			else if (pDesc->GraphicsPipeline.RasterizerState.CullMode == CULL_MODE_FRONT)
				m_RasterizerState.CullMode = GL_FRONT;

			if (pDesc->GraphicsPipeline.RasterizerState.FrontCounterClockwise)
				m_RasterizerState.FrontFace = GL_CCW;
			else
				m_RasterizerState.FrontFace = GL_CW;

			m_RasterizerState.DepthClipEnable = pDesc->GraphicsPipeline.RasterizerState.DepthClipEnable;
			m_RasterizerState.DepthBias = (float)pDesc->GraphicsPipeline.RasterizerState.DepthBias;
			m_RasterizerState.DepthBiasClamp = pDesc->GraphicsPipeline.RasterizerState.DepthBiasClamp;
			m_RasterizerState.SlopeScaleDepthBias = pDesc->GraphicsPipeline.RasterizerState.SlopeScaleDepthBias;

			m_RasterizerState.AntialiasedLineEnable = pDesc->GraphicsPipeline.RasterizerState.AntialiasedLineEnable;
			m_RasterizerState.MultisampleEnable = pDesc->GraphicsPipeline.RasterizerState.MultisampleEnable;
			m_RasterizerState.ScissorEnable = pDesc->GraphicsPipeline.RasterizerState.ScissorEnable;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLPipelineState::CreateBlendState(const PipelineStateDesc* pDesc)
		{
			m_BlendState.AlphaToCoverageEnable = pDesc->GraphicsPipeline.BlendState.AlphaToCoverageEnable;
			m_BlendState.IndependentBlendEnable = pDesc->GraphicsPipeline.BlendState.IndependentBlendEnable;
			m_BlendState.LogicOpEnable = pDesc->GraphicsPipeline.BlendState.LogicOpEnable;
			
			for (uint32 i = 0; i < 4; i++)
				m_BlendState.BlendFactor[i] = pDesc->GraphicsPipeline.BlendState.BlendFactor[i];

			for (uint32 i = 0; i < RE_MAX_RENDERTARGETS; i++)
			{
				m_BlendState.RenderTargets[i].blendEnable = pDesc->GraphicsPipeline.BlendState.RenderTargets[i].BlendEnable;

				m_BlendState.RenderTargets[i].SrcBlend = BlendTypeToGL(pDesc->GraphicsPipeline.BlendState.RenderTargets[i].SrcBlend);
				m_BlendState.RenderTargets[i].DstBlend = BlendTypeToGL(pDesc->GraphicsPipeline.BlendState.RenderTargets[i].DstBlend);
				m_BlendState.RenderTargets[i].SrcAlphaBlend = BlendTypeToGL(pDesc->GraphicsPipeline.BlendState.RenderTargets[i].SrcAlphaBlend);
				m_BlendState.RenderTargets[i].DstAlphaBlend = BlendTypeToGL(pDesc->GraphicsPipeline.BlendState.RenderTargets[i].DstAlphaBlend);

				m_BlendState.RenderTargets[i].BlendOperation = BlendOperationToGL(pDesc->GraphicsPipeline.BlendState.RenderTargets[i].BlendOperation);
				m_BlendState.RenderTargets[i].AlphaBlendOperation = BlendOperationToGL(pDesc->GraphicsPipeline.BlendState.RenderTargets[i].AlphaBlendOperation);


				if (pDesc->GraphicsPipeline.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_RED)
					m_BlendState.RenderTargets[i].WriteMask[0] = GL_TRUE;
				else
					m_BlendState.RenderTargets[i].WriteMask[0] = GL_FALSE;

				if (pDesc->GraphicsPipeline.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_GREEN)
					m_BlendState.RenderTargets[i].WriteMask[1] = GL_TRUE;
				else
					m_BlendState.RenderTargets[i].WriteMask[1] = GL_FALSE;

				if (pDesc->GraphicsPipeline.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_BLUE)
					m_BlendState.RenderTargets[i].WriteMask[2] = GL_TRUE;
				else
					m_BlendState.RenderTargets[i].WriteMask[2] = GL_FALSE;

				if (pDesc->GraphicsPipeline.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_ALPHA)
					m_BlendState.RenderTargets[i].WriteMask[3] = GL_TRUE;
				else
					m_BlendState.RenderTargets[i].WriteMask[3] = GL_FALSE;
			}
		}
	}
}