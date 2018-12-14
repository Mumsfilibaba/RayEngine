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
				if (m_Desc.Graphics.InputLayout.pElements != nullptr)
				{
					delete[] m_Desc.Graphics.InputLayout.pElements;
					m_Desc.Graphics.InputLayout.pElements = nullptr;

					m_Desc.Graphics.InputLayout.ElementCount = 0;
				}
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
			if (pDesc->Type == PIPELINE_TYPE_GRAPHICS)
				CreateGraphicsPipeline(pDesc);
			else if (pDesc->Type == PIPELINE_TYPE_COMPUTE)
				CreateComputePipeline(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLPipelineState::CreateGraphicsPipeline(const PipelineStateDesc* pDesc)
		{
			m_Program = glCreateProgram();

			GLShader* pShader = nullptr;
			if (pDesc->Graphics.pVertexShader != nullptr)
			{
				pShader = reinterpret_cast<GLShader*>(pDesc->Graphics.pVertexShader);
				glAttachShader(m_Program, pShader->GetGLShaderID());
			}
			if (pDesc->Graphics.pHullShader != nullptr)
			{
				pShader = reinterpret_cast<GLShader*>(pDesc->Graphics.pHullShader);
				glAttachShader(m_Program, pShader->GetGLShaderID());
			}
			if (pDesc->Graphics.pDomainShader != nullptr)
			{
				pShader = reinterpret_cast<GLShader*>(pDesc->Graphics.pDomainShader);
				glAttachShader(m_Program, pShader->GetGLShaderID());
			}
			if (pDesc->Graphics.pGeometryShader != nullptr)
			{
				pShader = reinterpret_cast<GLShader*>(pDesc->Graphics.pGeometryShader);
				glAttachShader(m_Program, pShader->GetGLShaderID());
			}
			if (pDesc->Graphics.pPixelShader != nullptr)
			{
				pShader = reinterpret_cast<GLShader*>(pDesc->Graphics.pPixelShader);
				glAttachShader(m_Program, pShader->GetGLShaderID());
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
			if (pDesc->Graphics.pGeometryShader != nullptr)
			{
				GLShader* pShader = reinterpret_cast<GLShader*>(pDesc->Graphics.pGeometryShader);
				glAttachShader(m_Program, pShader->GetGLShaderID());
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
				if (len > 0)
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
		void GLPipelineState::CreateInputLayout(const PipelineStateDesc* pDesc)
		{
			m_InputLayout.ElementCount = pDesc->Graphics.InputLayout.ElementCount;
			m_InputLayout.pElements = new GLInputLayoutElement[m_InputLayout.ElementCount];

			InputElementDesc* pElements = pDesc->Graphics.InputLayout.pElements;
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
			m_DepthState.DepthEnable = pDesc->Graphics.DepthStencilState.DepthEnable;
			m_DepthState.DepthFunc = ComparisonFuncToGL(pDesc->Graphics.DepthStencilState.DepthFunc);
			
			if (pDesc->Graphics.DepthStencilState.DepthWriteMask == DEPTH_WRITE_MASK_ALL)
				m_DepthState.DepthMask = GL_TRUE;
			else if (pDesc->Graphics.DepthStencilState.DepthWriteMask == DEPTH_WRITE_MASK_ZERO)
				m_DepthState.DepthMask = GL_FALSE;
			else
				m_DepthState.DepthMask = 0;

			m_DepthState.StencilEnable = pDesc->Graphics.DepthStencilState.StencilEnable;
			m_DepthState.WriteMask = pDesc->Graphics.DepthStencilState.StencilWriteMask;

			m_DepthState.FrontFace.StencilFunc = ComparisonFuncToGL(pDesc->Graphics.DepthStencilState.FrontFace.StencilFunc);
			m_DepthState.FrontFace.ReadMask = pDesc->Graphics.DepthStencilState.StencilReadMask;
			m_DepthState.FrontFace.StencilFailOp = StencilOpToGL(pDesc->Graphics.DepthStencilState.FrontFace.StencilFailOperation);
			m_DepthState.FrontFace.DepthFailOp = StencilOpToGL(pDesc->Graphics.DepthStencilState.FrontFace.StencilDepthFailOperation);
			m_DepthState.FrontFace.PassOp = StencilOpToGL(pDesc->Graphics.DepthStencilState.FrontFace.StencilPassOperation);

			m_DepthState.BackFace.StencilFunc = ComparisonFuncToGL(pDesc->Graphics.DepthStencilState.BackFace.StencilFunc);
			m_DepthState.BackFace.ReadMask = pDesc->Graphics.DepthStencilState.StencilReadMask;
			m_DepthState.BackFace.StencilFailOp = StencilOpToGL(pDesc->Graphics.DepthStencilState.BackFace.StencilFailOperation);
			m_DepthState.BackFace.DepthFailOp = StencilOpToGL(pDesc->Graphics.DepthStencilState.BackFace.StencilDepthFailOperation);
			m_DepthState.BackFace.PassOp = StencilOpToGL(pDesc->Graphics.DepthStencilState.BackFace.StencilPassOperation);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLPipelineState::CreateRasterizerState(const PipelineStateDesc* pDesc)
		{
			m_RasterizerState.ConservativeRasterizerEnable = pDesc->Graphics.RasterizerState.ConservativeRasterizerEnable;
			
			if (pDesc->Graphics.RasterizerState.FillMode == FILL_MODE_SOLID)
				m_RasterizerState.PolygonMode = GL_FILL;
			else if (pDesc->Graphics.RasterizerState.FillMode == FILL_MODE_WIREFRAME)
				m_RasterizerState.PolygonMode = GL_LINE;
			
			if (pDesc->Graphics.RasterizerState.CullMode == CULL_MODE_BACK)
				m_RasterizerState.CullMode = GL_BACK;
			else if (pDesc->Graphics.RasterizerState.CullMode == CULL_MODE_FRONT)
				m_RasterizerState.CullMode = GL_FRONT;

			if (pDesc->Graphics.RasterizerState.FrontCounterClockwise)
				m_RasterizerState.FrontFace = GL_CCW;
			else
				m_RasterizerState.FrontFace = GL_CW;

			m_RasterizerState.DepthClipEnable = pDesc->Graphics.RasterizerState.DepthClipEnable;
			m_RasterizerState.DepthBias = (float)pDesc->Graphics.RasterizerState.DepthBias;
			m_RasterizerState.DepthBiasClamp = pDesc->Graphics.RasterizerState.DepthBiasClamp;
			m_RasterizerState.SlopeScaleDepthBias = pDesc->Graphics.RasterizerState.SlopeScaleDepthBias;

			m_RasterizerState.AntialiasedLineEnable = pDesc->Graphics.RasterizerState.AntialiasedLineEnable;
			m_RasterizerState.MultisampleEnable = pDesc->Graphics.RasterizerState.MultisampleEnable;
			m_RasterizerState.ScissorEnable = pDesc->Graphics.RasterizerState.ScissorEnable;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLPipelineState::CreateBlendState(const PipelineStateDesc* pDesc)
		{
			m_BlendState.AlphaToCoverageEnable = pDesc->Graphics.BlendState.AlphaToCoverageEnable;
			m_BlendState.IndependentBlendEnable = pDesc->Graphics.BlendState.IndependentBlendEnable;
			m_BlendState.LogicOpEnable = pDesc->Graphics.BlendState.LogicOpEnable;
			
			for (uint32 i = 0; i < 4; i++)
				m_BlendState.BlendFactor[i] = pDesc->Graphics.BlendState.BlendFactor[i];

			for (uint32 i = 0; i < RE_MAX_RENDERTARGETS; i++)
			{
				m_BlendState.RenderTargets[i].blendEnable = pDesc->Graphics.BlendState.RenderTargets[i].BlendEnable;

				m_BlendState.RenderTargets[i].SrcBlend = BlendTypeToGL(pDesc->Graphics.BlendState.RenderTargets[i].SrcBlend);
				m_BlendState.RenderTargets[i].DstBlend = BlendTypeToGL(pDesc->Graphics.BlendState.RenderTargets[i].DstBlend);
				m_BlendState.RenderTargets[i].SrcAlphaBlend = BlendTypeToGL(pDesc->Graphics.BlendState.RenderTargets[i].SrcAlphaBlend);
				m_BlendState.RenderTargets[i].DstAlphaBlend = BlendTypeToGL(pDesc->Graphics.BlendState.RenderTargets[i].DstAlphaBlend);

				m_BlendState.RenderTargets[i].BlendOperation = BlendOperationToGL(pDesc->Graphics.BlendState.RenderTargets[i].BlendOperation);
				m_BlendState.RenderTargets[i].AlphaBlendOperation = BlendOperationToGL(pDesc->Graphics.BlendState.RenderTargets[i].AlphaBlendOperation);


				if (pDesc->Graphics.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_RED)
					m_BlendState.RenderTargets[i].WriteMask[0] = GL_TRUE;
				else
					m_BlendState.RenderTargets[i].WriteMask[0] = GL_FALSE;

				if (pDesc->Graphics.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_GREEN)
					m_BlendState.RenderTargets[i].WriteMask[1] = GL_TRUE;
				else
					m_BlendState.RenderTargets[i].WriteMask[1] = GL_FALSE;

				if (pDesc->Graphics.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_BLUE)
					m_BlendState.RenderTargets[i].WriteMask[2] = GL_TRUE;
				else
					m_BlendState.RenderTargets[i].WriteMask[2] = GL_FALSE;

				if (pDesc->Graphics.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_ALPHA)
					m_BlendState.RenderTargets[i].WriteMask[3] = GL_TRUE;
				else
					m_BlendState.RenderTargets[i].WriteMask[3] = GL_FALSE;
			}
		}
	}
}