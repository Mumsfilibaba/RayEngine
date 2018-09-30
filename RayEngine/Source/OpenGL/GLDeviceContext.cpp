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

#include "..\..\Include\OpenGL\GLDeviceContext.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		GLDeviceContext::GLDeviceContext(IDevice* pDevice, bool isDeffered)
			: mReferences(0)
		{
			AddRef();
			m_Device = reinterpret_cast<GLDevice*>(pDevice);
		}



		/////////////////////////////////////////////////////////////
		GLDeviceContext::~GLDeviceContext()
		{
		}



		/////////////////////////////////////////////////////////////
		void GLDeviceContext::ClearRendertargetView(IRenderTargetView* pView, float pColor[4]) const
		{
			if (pView == nullptr)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				
				glClearColorf(pColor[0], pColor[1], pColor[2], pColor[3]);
				glClear(GL_COLOR_BUFFER_BIT);
			}
		}



		/////////////////////////////////////////////////////////////
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



		/////////////////////////////////////////////////////////////
		void GLDeviceContext::SetSwapChain(ISwapchain* pSwapChain) const
		{
		}



		/////////////////////////////////////////////////////////////
		void GLDeviceContext::SetRendertargets(IRenderTargetView* pRenderTarget, IDepthStencilView* pDepthStencil) const
		{
		}



		/////////////////////////////////////////////////////////////
		void GLDeviceContext::SetShaderResourceViews(IShaderResourceView* pShaderResourceView, int32 startRootIndex) const
		{
		}



		/////////////////////////////////////////////////////////////
		void GLDeviceContext::SetUnorderedAccessViews(IUnorderedAccessView* pUnorderedAccessView, int32 startRootIndex) const
		{
		}



		/////////////////////////////////////////////////////////////
		void GLDeviceContext::SetConstantBuffers(IBuffer* pBuffer, int32 startRootIndex) const
		{
		}



		/////////////////////////////////////////////////////////////
		void GLDeviceContext::SetSamplers(ISampler* pSampler, int32 startRootIndex) const
		{
		}



		/////////////////////////////////////////////////////////////
		void GLDeviceContext::SetPipelineState(IPipelineState* pPipelineState) const
		{
		}



		/////////////////////////////////////////////////////////////
		void GLDeviceContext::SetRootLayout(IRootLayout* pRootLayout) const
		{
		}



		/////////////////////////////////////////////////////////////
		void GLDeviceContext::SetVertexBuffers(IBuffer* pBuffer, int32 startSlot) const
		{
		}



		/////////////////////////////////////////////////////////////
		void GLDeviceContext::SetViewports(const Viewport& viewport) const
		{
		}



		/////////////////////////////////////////////////////////////
		void GLDeviceContext::SetPrimitiveTopology(PRIMITIVE_TOPOLOGY topology) const
		{
		}



		/////////////////////////////////////////////////////////////
		void GLDeviceContext::SetScissorRects(const Math::Rectangle& rect) const
		{
		}



		/////////////////////////////////////////////////////////////
		void GLDeviceContext::Draw(int32 startVertex, int32 vertexCount) const
		{
		}



		/////////////////////////////////////////////////////////////
		void GLDeviceContext::DrawIndexed(int32 startVertex, int32 startIndex, int32 indexCount) const
		{
		}



		/////////////////////////////////////////////////////////////
		void GLDeviceContext::DrawInstanced(int32 startVertex, int32 vertexCount, int32 startInstance, int32 instanceCount) const
		{
		}



		/////////////////////////////////////////////////////////////
		void GLDeviceContext::DrawIndexInstanced(int32 startVertex, int32 startIndex, int32 indexCount, int32 startInstance, int32 instanceCount) const
		{
		}



		/////////////////////////////////////////////////////////////
		void GLDeviceContext::Dispatch(int32 threadGroupCountX, int32 threadGroupCountY, int32 threadGroupCountZ) const
		{
		}



		/////////////////////////////////////////////////////////////
		void GLDeviceContext::Flush() const
		{
		}



		/////////////////////////////////////////////////////////////
		void GLDeviceContext::ExecuteDefferedContext(IDeviceContext* pDefferedContext) const
		{
		}



		/////////////////////////////////////////////////////////////
		void GLDeviceContext::SetName(const std::string& name)
		{
		}



		/////////////////////////////////////////////////////////////
		void GLDeviceContext::QueryDevice(IDevice** ppDevice) const
		{
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType GLDeviceContext::GetReferenceCount() const
		{
			return mReferences;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType GLDeviceContext::Release()
		{
			IObject::CounterType counter = mReferences--;
			if (mReferences < 1)
				delete this;

			return counter;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType GLDeviceContext::AddRef()
		{
			mReferences++;
			return mReferences;
		}



		/////////////////////////////////////////////////////////////
		void GLDeviceContext::Create(bool isDeffered)
		{
		}
	}
}