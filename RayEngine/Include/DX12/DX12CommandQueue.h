#pragma once

#include "..\..\Include\Graphics\ICommandQueue.h"
#include "DX12Common.h"

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
			DX12CommandQueue(ID3D12Device* device, const CommanQueueInfo& info);
			DX12CommandQueue(DX12CommandQueue&& other);
			~DX12CommandQueue();

			void TransitionResource(const ITexture* resource, RESOURCE_STATE from,
				RESOURCE_STATE to, int32 subresource) const  override final;
			
			void Execute() const override final;
			bool Reset() const override final;
			bool Close() const override final;

			DX12CommandQueue& operator=(DX12CommandQueue&& other);

			ID3D12CommandQueue* GetCommandQueue() const;

		private:
			void Create(ID3D12Device* device, const CommanQueueInfo& info);

		private:
			ID3D12CommandQueue* m_Queue;
			ID3D12CommandAllocator* m_Allocator;
			ID3D12GraphicsCommandList* m_List;
		};
	}
}