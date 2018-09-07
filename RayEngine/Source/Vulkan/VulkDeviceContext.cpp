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

#include "..\..\Include\Vulkan\VulkDeviceContext.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////
		VulkDeviceContext::VulkDeviceContext(IDevice* pDevice, bool isDeffered)
			: m_References(0)
		{
		}



		//////////////////////////////////////////////////////////////
		VulkDeviceContext::~VulkDeviceContext()
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkDeviceContext::ClearRendertargetView(IRenderTargetView* pView, float pColor[4]) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkDeviceContext::ClearDepthStencilView(IDepthStencilView* pView, float depth, uint8 stencil) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkDeviceContext::SetRendertargets(IRenderTargetView* pRenderTarget, IDepthStencilView * pDepthStencil) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkDeviceContext::SetShaderResourceViews(IShaderResourceView* pShaderResourceView, int32 startRootIndex) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkDeviceContext::SetUnorderedAccessViews(IUnorderedAccessView* pUnorderedAccessView, int32 startRootIndex) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkDeviceContext::SetConstantBuffers(IBuffer* pBuffer, int32 startRootIndex) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkDeviceContext::SetSamplers(ISampler* pSampler, int32 startRootIndex) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkDeviceContext::SetPipelineState(IPipelineState* pPipelineState) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkDeviceContext::SetRootLayout(IRootLayout* pRootLayout) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkDeviceContext::SetVertexBuffers(IBuffer* pBuffer, int32 startSlot) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkDeviceContext::SetViewports(const Viewport& viewport) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkDeviceContext::SetPrimitiveTopology(PRIMITIVE_TOPOLOGY topology) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkDeviceContext::SetScissorRects(const Math::Rectangle& rect) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkDeviceContext::Draw(int32 startVertex, int32 vertexCount) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkDeviceContext::DrawIndexed(int32 startVertex, int32 startIndex, int32 indexCount) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkDeviceContext::DrawInstanced(int32 startVertex, int32 vertexCount, int32 startInstance, int32 instanceCount) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkDeviceContext::DrawIndexInstanced(int32 startVertex, int32 startIndex, int32 indexCount, int32 startInstance, int32 instanceCount) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkDeviceContext::Dispatch(int32 threadGroupCountX, int32 threadGroupCountY, int32 threadGroupCountZ) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkDeviceContext::Flush() const
		{
		}



		//////////////////////////////////////////////////////////////
		bool VulkDeviceContext::Reset() const
		{
			return false;
		}



		//////////////////////////////////////////////////////////////
		bool VulkDeviceContext::Close() const
		{
			return false;
		}



		//////////////////////////////////////////////////////////////
		void VulkDeviceContext::ExecuteDefferedContext(IDeviceContext* pDefferedContext) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkDeviceContext::SetName(const std::string& name)
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkDeviceContext::QueryDevice(IDevice** ppDevice) const
		{
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType VulkDeviceContext::GetReferenceCount() const
		{
			return m_References;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType VulkDeviceContext::Release()
		{
			IObject::CounterType counter = m_References--;
			if (m_References < 1)
				delete this;

			return counter;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType VulkDeviceContext::AddRef()
		{
			m_References++;
			return m_References;
		}
	}
}