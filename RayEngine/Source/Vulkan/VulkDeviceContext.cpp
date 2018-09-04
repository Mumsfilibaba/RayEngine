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
		VulkanDeviceContext::VulkanDeviceContext(IDevice* pDevice, bool isDeffered)
		{
		}



		//////////////////////////////////////////////////////////////
		VulkanDeviceContext::~VulkanDeviceContext()
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkanDeviceContext::ClearRendertargetView(IRenderTargetView* pView, float pColor[4]) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkanDeviceContext::ClearDepthStencilView(IDepthStencilView* pView, float depth, uint8 stencil) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkanDeviceContext::SetRendertargets(IRenderTargetView* pRenderTarget, IDepthStencilView * pDepthStencil) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkanDeviceContext::SetShaderResourceViews(IShaderResourceView* pShaderResourceView, int32 startRootIndex) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkanDeviceContext::SetUnorderedAccessViews(IUnorderedAccessView* pUnorderedAccessView, int32 startRootIndex) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkanDeviceContext::SetConstantBuffers(IBuffer* pBuffer, int32 startRootIndex) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkanDeviceContext::SetSamplers(ISampler* pSampler, int32 startRootIndex) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkanDeviceContext::SetPipelineState(IPipelineState* pPipelineState) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkanDeviceContext::SetRootLayout(IRootLayout* pRootLayout) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkanDeviceContext::SetVertexBuffers(IBuffer* pBuffer, int32 startSlot) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkanDeviceContext::SetViewports(const Viewport& viewport) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkanDeviceContext::SetPrimitiveTopology(PRIMITIVE_TOPOLOGY topology) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkanDeviceContext::SetScissorRects(const Math::Rectangle& rect) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkanDeviceContext::Draw(int32 startVertex, int32 vertexCount) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkanDeviceContext::DrawIndexed(int32 startVertex, int32 startIndex, int32 indexCount) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkanDeviceContext::DrawInstanced(int32 startVertex, int32 vertexCount, int32 startInstance, int32 instanceCount) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkanDeviceContext::DrawIndexInstanced(int32 startVertex, int32 startIndex, int32 indexCount, int32 startInstance, int32 instanceCount) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkanDeviceContext::Dispatch(int32 threadGroupCountX, int32 threadGroupCountY, int32 threadGroupCountZ) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkanDeviceContext::Flush() const
		{
		}



		//////////////////////////////////////////////////////////////
		bool VulkanDeviceContext::Reset() const
		{
			return false;
		}



		//////////////////////////////////////////////////////////////
		bool VulkanDeviceContext::Close() const
		{
			return false;
		}



		//////////////////////////////////////////////////////////////
		void VulkanDeviceContext::ExecuteDefferedContext(IDeviceContext* pDefferedContext) const
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkanDeviceContext::SetName(const std::string& name)
		{
		}



		//////////////////////////////////////////////////////////////
		void VulkanDeviceContext::QueryDevice(IDevice** ppDevice) const
		{
		}
	}
}