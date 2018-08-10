#pragma once

#include "..\Graphics\IDevice.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX12Device;



		/////////////////////////////////////////////////////////////
		class DX12CommandQueue final : public ICommandQueue
		{
		public:
			DX12CommandQueue(const DX12CommandQueue& other) = delete;
			DX12CommandQueue& operator=(const DX12CommandQueue& other) = delete;
			DX12CommandQueue(DX12CommandQueue&& other) = delete;
			DX12CommandQueue& operator=(DX12CommandQueue&& other) = delete;

		public:
			DX12CommandQueue(IDevice* pDevice, const CommandQueueInfo& info);
			~DX12CommandQueue();

			ID3D12CommandQueue* GetD3D12CommandQueue() const;

			void ClearRendertargetView(IRenderTargetView* pView, float pColor[4]) const override final;
			void ClearDepthStencilView(IDepthStencilView* pView, float depth, uint8 stencil) const override final;

			void SetPipelineState(IPipelineState* pPipelineState) const override final;
			void SetRootSignature(IRootSignature* pRootSignature) const override final;
			void SetVertexBuffers(IBuffer* pBuffer, int32 startSlot) const override final;

			//TODO: Multiple Viewports, rects and rendertargets
			void SetRendertargets(IRenderTargetView* pRenderTarget, IDepthStencilView* pDepthStencil) const override final;
			void SetViewports(const Viewport& viewport) const override final;
			void SetScissorRects(const Math::Rectangle& rect) const override final;
			void SetPrimitiveTopology(PRIMITIVE_TOPOLOGY topology) const override final;

			void CopyResource(ID3D12Resource* pDst, ID3D12Resource* pSrc) const;
			void CopyTextureRegion(ID3D12Resource* pDst, ID3D12Resource* pSrc, DXGI_FORMAT format,int32 width, int32 height, int32 depth, int32 stride) const;

			void TransitionResource(ITexture* pResource, RESOURCE_STATE to, int32 subresource) const override final;
			void TransitionResource(ID3D12Resource* pResource, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to, int32 subresource) const;

			void Draw(int32 startVertex, int32 vertexCount) const override final;
			void DrawIndexed(int32 startVertex, int32 startIndex, int32 indexCount) const override final;
			void DrawInstanced(int32 startVertex, int32 vertexCount, int32 startInstance, int32 instanceCount) const override final;
			void DrawIndexInstanced(int32 startVertex, int32 startIndex, int32 indexCount, int32 startInstance, int32 instanceCount) const override final;

			void Flush() const override final;
			void Execute() const override final;
			bool Reset() const override final;
			bool Close() const override final;

			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const CommandQueueInfo& info);

		private:
			DX12Device* m_Device;
			ID3D12CommandQueue* m_Queue;
			ID3D12CommandAllocator* m_Allocator;
			ID3D12GraphicsCommandList* m_List;
			ID3D12Fence* m_Fence;
			mutable uint64 m_CurrentFence;
		};
	}
}

#endif