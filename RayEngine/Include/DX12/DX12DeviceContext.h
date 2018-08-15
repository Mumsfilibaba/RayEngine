#pragma once

#include "..\Graphics\IDeviceContext.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX12Device;
		class DX12RootLayout;
		class DX12Resource;



		/////////////////////////////////////////////////////////////
		class DX12DeviceContext final : public IDeviceContext
		{
		public:
			DX12DeviceContext(const DX12DeviceContext& other) = delete;
			DX12DeviceContext& operator=(const DX12DeviceContext& other) = delete;
			DX12DeviceContext(DX12DeviceContext&& other) = delete;
			DX12DeviceContext& operator=(DX12DeviceContext&& other) = delete;

		public:
			DX12DeviceContext(IDevice* pDevice, bool isDeffered);
			~DX12DeviceContext();

			ID3D12Fence* GetD3D12Fence() const;
			ID3D12CommandList* GetD3D12CommandList() const;
			ID3D12CommandQueue* GetD3D12CommandQueue() const;
			ID3D12CommandAllocator* GetD3D12CommandAllocator() const;

			void TransitionResource(DX12Resource* pResource, D3D12_RESOURCE_STATES to, int32 subresource) const;

			void ClearRendertargetView(IRenderTargetView* pView, float pColor[4]) const override final;
			void ClearDepthStencilView(IDepthStencilView* pView, float depth, uint8 stencil) const override final;

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

			void Flush() const override final;
			bool Reset() const override final;
			bool Close() const override final;

			void ExecuteDefferedContext(IDeviceContext* pDefferedContext) const override final;

			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(bool isDeffered);
			void AddCommand() const;
			void ExecuteCommandList();

		private:
			DX12Device* m_Device;
			mutable DX12RootLayout* m_CurrentRootLayout;
			ID3D12CommandQueue* m_Queue;
			ID3D12CommandAllocator* m_Allocator;
			ID3D12GraphicsCommandList* m_CommandList;
			ID3D12Fence* m_Fence;
			bool m_IsDeffered;
			mutable uint64 m_CurrentFence;
			mutable uint32 m_NumCommands;
			uint32 m_MaxCommands;
		};
	}
}

#endif