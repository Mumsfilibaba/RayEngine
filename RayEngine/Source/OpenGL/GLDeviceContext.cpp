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
#include <Graphics/Viewport.h>
#include <OpenGL/GLDeviceContext.h>
#include <OpenGL/GLDevice.h>
#include <OpenGL/GLPipelineState.h>
#include <OpenGL/GLRootLayout.h>
#include <OpenGL/GLSwapchain.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLDeviceContext::GLDeviceContext(IDevice* pDevice, bool isDeffered)
			: m_Device(nullptr),
			m_CurrentRootLayout(nullptr),
			m_CurrentPipelineState(nullptr),
			m_pCurrentSwapchain(nullptr),
			m_VertexBuffers(),
			m_IsDeffered(false),
			m_VAOCache(),
			m_PrimitiveTopology(PRIMITIVE_TOPOLOGY_UNKNOWN),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<GLDevice*>(pDevice);

			Create(isDeffered);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLDeviceContext::~GLDeviceContext()
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceContext::ClearRendertargetView(IRenderTargetView* pView, float pColor[4]) const
		{
			if (pView == nullptr)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				
				glClearColorf(pColor[0], pColor[1], pColor[2], pColor[3]);
				glClear(GL_COLOR_BUFFER_BIT);
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceContext::ClearDepthStencilView(IDepthStencilView* pView, float depth, uint8 stencil) const
		{
			if (pView == nullptr)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				glClearDepthf(depth);
				glClearStencil(stencil);

				glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceContext::SetSwapChain(ISwapchain* pSwapChain) const
		{
			reinterpret_cast<GLSwapchain*>(pSwapChain)->MakeCurrent();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceContext::SetRendertargets(IRenderTargetView* pRenderTarget, IDepthStencilView* pDepthStencil) const
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceContext::SetShaderResourceViews(IShaderResourceView* pShaderResourceView, int32 startRootIndex) const
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceContext::SetUnorderedAccessViews(IUnorderedAccessView* pUnorderedAccessView, int32 startRootIndex) const
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceContext::SetConstantBuffers(IBuffer* pBuffer, int32 startRootIndex) const
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceContext::SetSamplers(ISampler* pSampler, int32 startRootIndex) const
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceContext::SetPipelineState(IPipelineState* pPipelineState) const
		{
			ReRelease_S(m_CurrentPipelineState);
			
			if (pPipelineState != nullptr)
			{
				m_CurrentPipelineState = pPipelineState->QueryReference<GLPipelineState>();

				SetPipelineState();
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceContext::SetRootLayout(IRootLayout* pRootLayout) const
		{
			ReRelease_S(m_CurrentRootLayout);

			if (pRootLayout != nullptr)
			{
				m_CurrentRootLayout = pRootLayout->QueryReference<GLRootLayout>();
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceContext::SetVertexBuffers(IBuffer* pBuffer, int32 startSlot) const
		{
			m_VertexBuffers[startSlot] = reinterpret_cast<GLBuffer*>(pBuffer);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceContext::SetViewports(const Viewport* pViewport) const
		{
			glViewport(static_cast<GLint>(pViewport->TopLeftX), static_cast<GLint>(pViewport->TopLeftY),
				static_cast<GLsizei>(pViewport->Width), static_cast<GLsizei>(pViewport->Height));
			glDepthRangef(pViewport->MinDepth, pViewport->MaxDepth);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceContext::SetPrimitiveTopology(PRIMITIVE_TOPOLOGY topology) const
		{
			m_PrimitiveTopology = PrimitiveTopologyToGL(topology);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceContext::SetScissorRects(const Rect* pRect) const
		{
			glScissor(pRect->Left, pRect->Top, pRect->Right, pRect->Bottom);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceContext::Draw(int32 startVertex, int32 vertexCount) const
		{
			uint32 vao = m_VAOCache.GetVAO(m_CurrentPipelineState, m_VertexBuffers, nullptr);
			
			glBindVertexArray(vao);
			glDrawArrays(m_PrimitiveTopology, startVertex, vertexCount);
			glBindVertexArray(0);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceContext::DrawIndexed(int32 startVertex, int32 startIndex, int32 indexCount) const
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceContext::DrawInstanced(int32 startVertex, int32 vertexCount, int32 startInstance, int32 instanceCount) const
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceContext::DrawIndexInstanced(int32 startVertex, int32 startIndex, int32 indexCount, int32 startInstance, int32 instanceCount) const
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceContext::Dispatch(int32 threadGroupCountX, int32 threadGroupCountY, int32 threadGroupCountZ) const
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceContext::Flush() const
		{
			//Not relevant?
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceContext::ExecuteDefferedContext(IDeviceContext* pDefferedContext) const
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType GLDeviceContext::Release()
		{
			CounterType counter = --m_References;
			if (m_References < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType GLDeviceContext::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceContext::Create(bool isDeffered)
		{
			m_IsDeffered = isDeffered;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLDeviceContext::SetPipelineState() const
		{
			glUseProgram(m_CurrentPipelineState->GetGLProgram());

			if (m_CurrentPipelineState->GetPipelineType() == PIPELINE_TYPE_GRAPHICS)
			{
				//Set Depthstate
				const GLDepthState* pDepthState = &m_CurrentPipelineState->GetGLDepthState();
				if (pDepthState->DepthEnable)
					glEnable(GL_DEPTH_TEST);
				else
					glDisable(GL_DEPTH_TEST);

				glDepthMask(pDepthState->DepthMask);
				glDepthFunc(pDepthState->DepthFunc);

				if (pDepthState->StencilEnable)
					glEnable(GL_STENCIL_TEST);
				else
					glDisable(GL_STENCIL_TEST);

				glStencilMask(pDepthState->WriteMask);

				glStencilOpSeparate(GL_FRONT, pDepthState->FrontFace.StencilFailOp, pDepthState->FrontFace.DepthFailOp, pDepthState->FrontFace.PassOp);
				glStencilFuncSeparate(GL_FRONT, pDepthState->FrontFace.StencilFunc, 0, pDepthState->FrontFace.ReadMask);
				glStencilOpSeparate(GL_BACK, pDepthState->BackFace.StencilFailOp, pDepthState->BackFace.DepthFailOp, pDepthState->BackFace.PassOp);
				glStencilFuncSeparate(GL_BACK, pDepthState->BackFace.StencilFunc, 0, pDepthState->BackFace.ReadMask);


				//Set RasterizerState
				const GLRasterizerState* pRasterizerState = &m_CurrentPipelineState->GetGLRasterizerState();
				if (m_Device->ExtensionSupported("GL_NV_conservative_raster"))
				{
					if (pRasterizerState->ConservativeRasterizerEnable)
						glEnable(GL_CONSERVATIVE_RASTERIZATION_NV);
					else
						glDisable(GL_CONSERVATIVE_RASTERIZATION_NV);
				}
				else
				{
					LOG_WARNING("OpenGL: GL_NV_conservative_raster not supported.");
				}

				glPolygonMode(GL_FRONT_AND_BACK, pRasterizerState->PolygonMode);
				glCullFace(pRasterizerState->CullMode);
				glFrontFace(pRasterizerState->FrontFace);

				if (pRasterizerState->DepthClipEnable)
					glDisable(GL_DEPTH_CLAMP);
				else
					glEnable(GL_DEPTH_CLAMP);

				if (glPolygonOffsetClamp != nullptr)
				{
					glPolygonOffsetClamp(pRasterizerState->SlopeScaleDepthBias, pRasterizerState->DepthBias, pRasterizerState->DepthBiasClamp);
				}
				else
				{
					glPolygonOffset(pRasterizerState->SlopeScaleDepthBias, pRasterizerState->DepthBias);
					LOG_WARNING("OpenGL: glPolygonOffsetClamp not supported.");
				}

				if (pRasterizerState->AntialiasedLineEnable)
				{
					glEnable(GL_LINE_SMOOTH);
					glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
				}
				else
				{
					glDisable(GL_LINE_SMOOTH);
				}

				if (pRasterizerState->ScissorEnable)
					glEnable(GL_SCISSOR_TEST);
				else
					glDisable(GL_SCISSOR_TEST);

				if (pRasterizerState->MultisampleEnable)
					glEnable(GL_MULTISAMPLE);
				else
					glDisable(GL_MULTISAMPLE);


				//Set Blendstate
				const GLBlendState* pBlendState = &m_CurrentPipelineState->GetGLBlendState();
				if (pBlendState->AlphaToCoverageEnable)
					glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
				else
					glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);

				if (pBlendState->LogicOpEnable)
					glEnable(GL_LOGIC_OP);
				else
					glDisable(GL_LOGIC_OP);

				glBlendColor(pBlendState->BlendFactor[0], pBlendState->BlendFactor[1], pBlendState->BlendFactor[2], pBlendState->BlendFactor[3]);
				
				if (pBlendState->IndependentBlendEnable && glBlendFuncSeparatei != nullptr && glBlendEquationSeparatei != nullptr)
				{
					for (uint32 i = 0; i < RE_MAX_RENDERTARGETS; i++)
					{
						if (pBlendState->RenderTargets[i].blendEnable == true)
						{
							glEnablei(GL_BLEND, i);

							glBlendFuncSeparatei(i, pBlendState->RenderTargets[i].SrcBlend, pBlendState->RenderTargets[i].DstBlend, 
								pBlendState->RenderTargets[i].SrcAlphaBlend, pBlendState->RenderTargets[i].DstAlphaBlend);
							glBlendEquationSeparatei(i, pBlendState->RenderTargets[i].BlendOperation, pBlendState->RenderTargets[i].AlphaBlendOperation);

							glColorMaski(i, pBlendState->RenderTargets[i].WriteMask[0], pBlendState->RenderTargets[i].WriteMask[1],
								pBlendState->RenderTargets[i].WriteMask[2], pBlendState->RenderTargets[i].WriteMask[3]);
						}
						else
						{
							glDisablei(GL_BLEND, i);
						}
					}
				}
				else
				{
					if (pBlendState->RenderTargets[0].blendEnable == true)
					{
						glEnable(GL_BLEND);
						
						glBlendFuncSeparate(pBlendState->RenderTargets[0].SrcBlend, pBlendState->RenderTargets[0].DstBlend, 
							pBlendState->RenderTargets[0].SrcAlphaBlend, pBlendState->RenderTargets[0].DstAlphaBlend);
						glBlendEquationSeparate(pBlendState->RenderTargets[0].BlendOperation, pBlendState->RenderTargets[0].AlphaBlendOperation);
					
						glColorMask(pBlendState->RenderTargets[0].WriteMask[0], pBlendState->RenderTargets[0].WriteMask[1],
							pBlendState->RenderTargets[0].WriteMask[2], pBlendState->RenderTargets[0].WriteMask[3]);
					}
					else
					{
						glDisable(GL_BLEND);
					}
				}
			}
		}
	}
}