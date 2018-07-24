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

			//TODO: Copy resource


			//TODO: Transition DX12Resource
			void TransitionResource(const DX12Resource& resource, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to, int32 subresource) const;
			void TransitionResource(const ITexture* pResource, RESOURCE_STATE from, RESOURCE_STATE to, int32 subresource) const  override final;

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