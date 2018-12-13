/*////////////////////////////////////////////////////////////

Copyright 2018 Alexander Dahlin

Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in
compliance with the License. You may obtain a copy of
the License at

http ://www.apache.org/licenses/LICENSE-2.0

THIS SOFTWARE IS PROVIDED "AS IS". MEANING NO WARRANTY
OR SUPPORT IS PROVIDED OF ANY KIND.

In event of any damages, direct or indirect that can
be traced back to the use of this software, shall no
contributor be held liable. This includes computer
failure and or malfunction of any kind.

////////////////////////////////////////////////////////////*/

#include <RayEngine.h>
#include <OpenGL/GLDevice.h>
#include <OpenGL/GLShader.h>
#include <OpenGL/GLPipelineState.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLPipelineState::GLPipelineState(IDevice* pDevice, const PipelineStateDesc* pDesc)
			: m_Device(nullptr),
			m_VS(nullptr),
			m_HS(nullptr),
			m_DS(nullptr),
			m_GS(nullptr),
			m_PS(nullptr),
			m_CS(nullptr),
			m_InputLayout(),
			m_DepthState(),
			m_RasterizerState(),
			m_BlendState(),
			m_Desc(),
			m_Program(0),
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

			if (m_Desc.Type == PIPELINE_TYPE_GRAPHICS)
			{
				if (m_Desc.GraphicsPipeline.InputLayout.pElements != nullptr)
				{
					delete[] m_Desc.GraphicsPipeline.InputLayout.pElements;
					m_Desc.GraphicsPipeline.InputLayout.pElements = nullptr;

					m_Desc.GraphicsPipeline.InputLayout.ElementCount = 0;
				}

				ReRelease_S(m_VS);
				ReRelease_S(m_HS);
				ReRelease_S(m_DS);
				ReRelease_S(m_GS);
				ReRelease_S(m_PS);
			}
			else
			{
				ReRelease_S(m_CS);
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLPipelineState::GetDesc(PipelineStateDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType GLPipelineState::Release()
		{
			CounterType refs = --m_References;
			if (refs < 1)
				delete this;

			return refs;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType GLPipelineState::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLPipelineState::Create(const PipelineStateDesc* pDesc)
		{
			CopyPipelineStateDesc(&m_Desc, pDesc);

			if (pDesc->Type == PIPELINE_TYPE_GRAPHICS)
				CreateGraphicsPipeline();
			else if (pDesc->Type == PIPELINE_TYPE_COMPUTE)
				CreateComputePipeline();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLPipelineState::CreateGraphicsPipeline()
		{
			m_Program = glCreateProgram();

			if (m_Desc.GraphicsPipeline.pVertexShader != nullptr)
			{
				m_VS = m_Desc.GraphicsPipeline.pVertexShader->QueryReference<GLShader>();
				glAttachShader(m_Program, m_VS->GetGLShaderID());
			}
			if (m_Desc.GraphicsPipeline.pHullShader != nullptr)
			{
				m_HS = m_Desc.GraphicsPipeline.pHullShader->QueryReference<GLShader>();
				glAttachShader(m_Program, m_HS->GetGLShaderID());
			}
			if (m_Desc.GraphicsPipeline.pDomainShader != nullptr)
			{
				m_DS = m_Desc.GraphicsPipeline.pDomainShader->QueryReference<GLShader>();
				glAttachShader(m_Program, m_DS->GetGLShaderID());
			}
			if (m_Desc.GraphicsPipeline.pGeometryShader != nullptr)
			{
				m_GS = m_Desc.GraphicsPipeline.pGeometryShader->QueryReference<GLShader>();
				glAttachShader(m_Program, m_GS->GetGLShaderID());
			}
			if (m_Desc.GraphicsPipeline.pPixelShader != nullptr)
			{
				m_PS = m_Desc.GraphicsPipeline.pPixelShader->QueryReference<GLShader>();
				glAttachShader(m_Program, m_PS->GetGLShaderID());
			}

			LinkShaders();
			CreateInputLayout();
			CreateDepthState();
			CreateRasterizerState();
			CreateBlendState();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLPipelineState::CreateComputePipeline()
		{
			m_Program = glCreateProgram();
			if (m_Desc.GraphicsPipeline.pGeometryShader != nullptr)
			{
				m_GS = m_Desc.GraphicsPipeline.pGeometryShader->QueryReference<GLShader>();
				glAttachShader(m_Program, m_GS->GetGLShaderID());
			}

			LinkShaders();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLPipelineState::LinkShaders()
		{
			glLinkProgram(m_Program);

			int32 result = GL_TRUE;
			glGetProgramiv(m_Program, GL_LINK_STATUS, &result);
			if (result != GL_TRUE)
			{
				int32 len = 0;
				glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &len);

				std::string message = "OpenGL: Could not link program.\n";
				if (len != 0)
				{
					std::vector<char> log;
					log.resize(len);
					glGetProgramInfoLog(m_Program, len, &len, log.data());

					message += log.data();
				}

				LOG_ERROR(message);
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLPipelineState::CreateInputLayout()
		{
			m_InputLayout.ElementCount = m_Desc.GraphicsPipeline.InputLayout.ElementCount;
			m_InputLayout.pElements = new GLInputLayoutElement[m_InputLayout.ElementCount];

			InputElementDesc* pElements = m_Desc.GraphicsPipeline.InputLayout.pElements;
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
		void GLPipelineState::CreateDepthState()
		{
			m_DepthState.DepthEnable = m_Desc.GraphicsPipeline.DepthStencilState.DepthEnable;
			m_DepthState.DepthFunc = ComparisonFuncToGL(m_Desc.GraphicsPipeline.DepthStencilState.DepthFunc);
			
			if (m_Desc.GraphicsPipeline.DepthStencilState.DepthWriteMask == DEPTH_WRITE_MASK_ALL)
				m_DepthState.DepthMask = GL_TRUE;
			else if (m_Desc.GraphicsPipeline.DepthStencilState.DepthWriteMask == DEPTH_WRITE_MASK_ZERO)
				m_DepthState.DepthMask = GL_FALSE;
			else
				m_DepthState.DepthMask = 0;

			m_DepthState.StencilEnable = m_Desc.GraphicsPipeline.DepthStencilState.StencilEnable;
			m_DepthState.WriteMask = m_Desc.GraphicsPipeline.DepthStencilState.StencilWriteMask;

			m_DepthState.FrontFace.StencilFunc = ComparisonFuncToGL(m_Desc.GraphicsPipeline.DepthStencilState.FrontFace.StencilFunc);
			m_DepthState.FrontFace.ReadMask = m_Desc.GraphicsPipeline.DepthStencilState.StencilReadMask;
			m_DepthState.FrontFace.StencilFailOp = StencilOpToGL(m_Desc.GraphicsPipeline.DepthStencilState.FrontFace.StencilFailOperation);
			m_DepthState.FrontFace.DepthFailOp = StencilOpToGL(m_Desc.GraphicsPipeline.DepthStencilState.FrontFace.StencilDepthFailOperation);
			m_DepthState.FrontFace.PassOp = StencilOpToGL(m_Desc.GraphicsPipeline.DepthStencilState.FrontFace.StencilPassOperation);

			m_DepthState.BackFace.StencilFunc = ComparisonFuncToGL(m_Desc.GraphicsPipeline.DepthStencilState.BackFace.StencilFunc);
			m_DepthState.BackFace.ReadMask = m_Desc.GraphicsPipeline.DepthStencilState.StencilReadMask;
			m_DepthState.BackFace.StencilFailOp = StencilOpToGL(m_Desc.GraphicsPipeline.DepthStencilState.BackFace.StencilFailOperation);
			m_DepthState.BackFace.DepthFailOp = StencilOpToGL(m_Desc.GraphicsPipeline.DepthStencilState.BackFace.StencilDepthFailOperation);
			m_DepthState.BackFace.PassOp = StencilOpToGL(m_Desc.GraphicsPipeline.DepthStencilState.BackFace.StencilPassOperation);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLPipelineState::CreateRasterizerState()
		{
			m_RasterizerState.ConservativeRasterizerEnable = m_Desc.GraphicsPipeline.RasterizerState.ConservativeRasterizerEnable;
			
			if (m_Desc.GraphicsPipeline.RasterizerState.FillMode == FILL_MODE_SOLID)
				m_RasterizerState.PolygonMode = GL_FILL;
			else if (m_Desc.GraphicsPipeline.RasterizerState.FillMode == FILL_MODE_WIREFRAME)
				m_RasterizerState.PolygonMode = GL_LINE;
			
			if (m_Desc.GraphicsPipeline.RasterizerState.CullMode == CULL_MODE_BACK)
				m_RasterizerState.CullMode = GL_BACK;
			else if (m_Desc.GraphicsPipeline.RasterizerState.CullMode == CULL_MODE_FRONT)
				m_RasterizerState.CullMode = GL_FRONT;

			if (m_Desc.GraphicsPipeline.RasterizerState.FrontCounterClockwise)
				m_RasterizerState.FrontFace = GL_CCW;
			else
				m_RasterizerState.FrontFace = GL_CW;

			m_RasterizerState.DepthClipEnable = m_Desc.GraphicsPipeline.RasterizerState.DepthClipEnable;
			m_RasterizerState.DepthBias = (float)m_Desc.GraphicsPipeline.RasterizerState.DepthBias;
			m_RasterizerState.DepthBiasClamp = m_Desc.GraphicsPipeline.RasterizerState.DepthBiasClamp;
			m_RasterizerState.SlopeScaleDepthBias = m_Desc.GraphicsPipeline.RasterizerState.SlopeScaleDepthBias;

			m_RasterizerState.AntialiasedLineEnable = m_Desc.GraphicsPipeline.RasterizerState.AntialiasedLineEnable;
			m_RasterizerState.MultisampleEnable = m_Desc.GraphicsPipeline.RasterizerState.MultisampleEnable;
			m_RasterizerState.ScissorEnable = m_Desc.GraphicsPipeline.RasterizerState.ScissorEnable;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLPipelineState::CreateBlendState()
		{
			m_BlendState.AlphaToCoverageEnable = m_Desc.GraphicsPipeline.BlendState.AlphaToCoverageEnable;
			m_BlendState.IndependentBlendEnable = m_Desc.GraphicsPipeline.BlendState.IndependentBlendEnable;
			m_BlendState.LogicOpEnable = m_Desc.GraphicsPipeline.BlendState.LogicOpEnable;
			
			for (uint32 i = 0; i < 4; i++)
				m_BlendState.BlendFactor[i] = m_Desc.GraphicsPipeline.BlendState.BlendFactor[i];

			for (uint32 i = 0; i < RE_MAX_RENDERTARGETS; i++)
			{
				m_BlendState.RenderTargets[i].blendEnable = m_Desc.GraphicsPipeline.BlendState.RenderTargets[i].BlendEnable;

				m_BlendState.RenderTargets[i].SrcBlend = BlendTypeToGL(m_Desc.GraphicsPipeline.BlendState.RenderTargets[i].SrcBlend);
				m_BlendState.RenderTargets[i].DstBlend = BlendTypeToGL(m_Desc.GraphicsPipeline.BlendState.RenderTargets[i].DstBlend);
				m_BlendState.RenderTargets[i].SrcAlphaBlend = BlendTypeToGL(m_Desc.GraphicsPipeline.BlendState.RenderTargets[i].SrcAlphaBlend);
				m_BlendState.RenderTargets[i].DstAlphaBlend = BlendTypeToGL(m_Desc.GraphicsPipeline.BlendState.RenderTargets[i].DstAlphaBlend);

				m_BlendState.RenderTargets[i].BlendOperation = BlendOperationToGL(m_Desc.GraphicsPipeline.BlendState.RenderTargets[i].BlendOperation);
				m_BlendState.RenderTargets[i].AlphaBlendOperation = BlendOperationToGL(m_Desc.GraphicsPipeline.BlendState.RenderTargets[i].AlphaBlendOperation);


				if (m_Desc.GraphicsPipeline.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_RED)
					m_BlendState.RenderTargets[i].WriteMask[0] = GL_TRUE;
				else
					m_BlendState.RenderTargets[i].WriteMask[0] = GL_FALSE;

				if (m_Desc.GraphicsPipeline.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_GREEN)
					m_BlendState.RenderTargets[i].WriteMask[1] = GL_TRUE;
				else
					m_BlendState.RenderTargets[i].WriteMask[1] = GL_FALSE;

				if (m_Desc.GraphicsPipeline.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_BLUE)
					m_BlendState.RenderTargets[i].WriteMask[2] = GL_TRUE;
				else
					m_BlendState.RenderTargets[i].WriteMask[2] = GL_FALSE;

				if (m_Desc.GraphicsPipeline.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_ALPHA)
					m_BlendState.RenderTargets[i].WriteMask[3] = GL_TRUE;
				else
					m_BlendState.RenderTargets[i].WriteMask[3] = GL_FALSE;
			}
		}
	}
}