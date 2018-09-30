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

#pragma once
#include <vector>
#include "..\Graphics\IDeviceContext.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12Common.h"

#define RE_DX12_MAX_COMMANDS 15

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX12Device;
		class DX12RootLayout;
		class DX12Resource;
		class DX12CommandList;



		/////////////////////////////////////////////////////////////
		class DX12DeviceContext final : public IDeviceContext
		{
			RE_IMPLEMENT_INTERFACE(DX12DeviceContext);

		public:
			DX12DeviceContext(IDevice* pDevice, bool isDeffered);
			~DX12DeviceContext();

			ID3D12Fence* GetD3D12Fence() const;
			
			ID3D12CommandQueue* GetD3D12CommandQueue() const;
			
			DX12CommandList* GetDX12List() const;
			
			DX12CommandList* GetDX12ComputeList() const;

			void CopyResource(DX12Resource* pDst, DX12Resource* pSrc) const;
			
			void CopyTexture(DX12Resource* pDst, DX12Resource* pSrc, DXGI_FORMAT format, int32 width, int32 height, int32 depth, int32 stride) const;
			
			void TransitionResource(DX12Resource* pResource, D3D12_RESOURCE_STATES to, int32 subresource) const;
			
			void TransitionResourceGroup(DX12Resource* const * ppResource, D3D12_RESOURCE_STATES* pToStates, int32* pSbresources, int32 count) const;
			
			void TransitionResourceIndirect(DX12Resource* pResource, D3D12_RESOURCE_STATES to, int32 subresource) const;
			
			void TransitionResourceGroupIndirect(DX12Resource* const * ppResource, D3D12_RESOURCE_STATES* pToStates, int32* pSbresources, int32 count) const;

			void ClearRendertargetView(IRenderTargetView* pView, float pColor[4]) const override final;
			
			void ClearDepthStencilView(IDepthStencilView* pView, float depth, uint8 stencil) const override final;

			void SetSwapChain(ISwapchain* pSwapChain) const override final;

			void SetRendertargets(IRenderTargetView* pRenderTarget, IDepthStencilView* pDepthStencil) const override final;
			
			void SetShaderResourceViews(IShaderResourceView* pShaderResourceView, int32 startRootIndex) const override final;
			
			void SetUnorderedAccessViews(IUnorderedAccessView* pUnorderedAccessView, int32 startRootIndex) const override final;
			
			void SetConstantBuffers(IBuffer* pBuffer, int32 startRootIndex) const override final;
			
			void SetSamplers(ISampler* pSampler, int32 startRootIndex) const override final;
			
			void SetPipelineState(IPipelineState* pPipelineState) const override final;
			
			void SetRootLayout(IRootLayout* pRootLayout) const override final;
			
			void SetVertexBuffers(IBuffer* pBuffer, int32 startSlot) const override final;
			
			void SetViewports(const Viewport& viewport) const override final;
			
			void SetPrimitiveTopology(PRIMITIVE_TOPOLOGY topology) const override final;
			
			void SetScissorRects(const Math::Rectangle& rect) const override final;

			void Draw(int32 startVertex, int32 vertexCount) const override final;
			
			void DrawIndexed(int32 startVertex, int32 startIndex, int32 indexCount) const override final;
			
			void DrawInstanced(int32 startVertex, int32 vertexCount, int32 startInstance, int32 instanceCount) const override final;
			
			void DrawIndexInstanced(int32 startVertex, int32 startIndex, int32 indexCount, int32 startInstance, int32 instanceCount) const override final;

			void Dispatch(int32 threadGroupCountX, int32 threadGroupCountY, int32 threadGroupCountZ) const override final;

			void Flush() const override final;
			
			bool Reset() const;
			
			bool Close() const;

			void ExecuteDefferedContext(IDeviceContext* pDefferedContext) const override final;

			void SetName(const std::string& name) override final;
			
			void QueryDevice(IDevice** ppDevice) const override final;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;


		private:
			void Create(bool isDeffered);
			
			void AddCommand() const;
			
			void ExecuteCommandLists() const;
			
			void CommitDefferedBarriers() const;

			D3D12_RESOURCE_BARRIER CreateTransitionBarrier(DX12Resource* pResource, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to, int32 subresource) const;


		private:
			DX12Device* m_Device;
			mutable DX12RootLayout* m_CurrentRootLayout;
			ID3D12CommandQueue* m_Queue;
			DX12CommandList* m_List;
			DX12CommandList* m_ComputeList;
			ID3D12Fence* m_Fence;
			
			bool m_IsDeffered;
			
			mutable uint64 m_CurrentFence;
			uint32 m_MaxCommands;
			mutable uint32 m_NumCommands;
			
			mutable std::vector<D3D12_RESOURCE_BARRIER> m_DefferedBarriers;

			IObject::CounterType mReferences;
		};
	}
}

#endif