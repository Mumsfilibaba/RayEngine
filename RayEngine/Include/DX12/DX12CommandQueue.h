#pragma once

#include "..\Graphics\IDevice.h"
#include "DX12Resource.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12CommandQueue : public ICommandQueue
		{
		public:
			DX12CommandQueue(const DX12CommandQueue& other) = delete;
			DX12CommandQueue& operator=(const DX12CommandQueue& other) = delete;

		public:
			DX12CommandQueue();
			DX12CommandQueue(const IDevice* pDevice, const CommandQueueInfo& info);
			DX12CommandQueue(DX12CommandQueue&& other);
			~DX12CommandQueue();

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

			//TODO: Copy resource
			void CopyResource(DX12Resource* dst, const DX12Resource* src) const;
			void CopyTextureRegion(DX12Resource* dst, const DX12Resource* src, DXGI_FORMAT format,
				int32 width, int32 height, int32 depth, int32 stride) const;

			void TransitionResource(DX12Resource* resource, D3D12_RESOURCE_STATES to, int32 subresource) const;
			void TransitionResource(ITexture* pResource, RESOURCE_STATE to, int32 subresource) const  override final;

			void Draw(int32 startVertex, int32 vertexCount) const override final;
			void DrawIndexed(int32 startVertex, int32 startIndex, int32 indexCount) const override final;
			void DrawInstanced(int32 startVertex, int32 vertexCount, int32 startInstance, int32 instanceCount) const override final;
			void DrawIndexInstanced(int32 startVertex, int32 startIndex, int32 indexCount, int32 startInstance, int32 instanceCount) const override final;

			void Flush() const override final;
			void Execute() const override final;
			bool Reset() const override final;
			bool Close() const override final;

			DX12CommandQueue& operator=(DX12CommandQueue&& other);

			ID3D12CommandQueue* GetD3D12CommandQueue() const;

		private:
			void Create(const IDevice* pDevice, const CommandQueueInfo& info);

		private:
			ID3D12CommandQueue* m_Queue;
			ID3D12CommandAllocator* m_Allocator;
			ID3D12GraphicsCommandList* m_List;
			ID3D12Fence* m_Fence;
			mutable uint64 m_CurrentFence;
		};
	}
}