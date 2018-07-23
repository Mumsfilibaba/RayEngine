#include "..\..\Include\DX12\DX12CommandQueue.h"
#include "..\..\Include\DX12\DX12Texture.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12CommandQueue::DX12CommandQueue(ID3D12Device* device, const CommanQueueInfo& info)
			: m_Queue(nullptr),
			m_Allocator(nullptr),
			m_List(nullptr),
			m_Fence(nullptr),
			m_CurrentFence(0)
		{
			Create(device, info);
		}



		/////////////////////////////////////////////////////////////
		DX12CommandQueue::DX12CommandQueue(DX12CommandQueue&& other)
			: m_Queue(other.m_Queue),
			m_Allocator(other.m_Allocator),
			m_List(other.m_List),
			m_Fence(other.m_Fence),
			m_CurrentFence(other.m_CurrentFence)
		{
			other.m_Queue = nullptr;
			other.m_Allocator = nullptr;
			other.m_List = nullptr;
			other.m_Fence = nullptr;
			other.m_CurrentFence = 0;
		}



		/////////////////////////////////////////////////////////////
		DX12CommandQueue::~DX12CommandQueue()
		{
			D3DRelease_S(m_Queue);
			D3DRelease_S(m_Allocator);
			D3DRelease_S(m_List);
			D3DRelease_S(m_Fence);
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::TransitionResource(const ITexture* resource, RESOURCE_STATE from,
			RESOURCE_STATE to, int32 subresource) const
		{
			const DX12Texture* res = reinterpret_cast<const DX12Texture*>(resource);

			D3D12_RESOURCE_BARRIER barrier = {};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

			barrier.Transition.Subresource = subresource;

			barrier.Transition.pResource = res->GetResource();

			barrier.Transition.StateBefore = ReToDXResourceState(from);
			barrier.Transition.StateAfter = ReToDXResourceState(to);

			m_List->ResourceBarrier(1, &barrier);
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::Flush() const
		{
			m_CurrentFence++;
			if (FAILED(m_Queue->Signal(m_Fence, m_CurrentFence)))
				return;

			if (m_Fence->GetCompletedValue() < m_CurrentFence)
			{
				HANDLE ev = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

				if (FAILED(m_Fence->SetEventOnCompletion(m_CurrentFence, ev)))
					return;

				WaitForSingleObject(ev, INFINITE);
				CloseHandle(ev);
			}

			return;
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::Execute() const
		{
			ID3D12CommandList* list = m_List;
			m_Queue->ExecuteCommandLists(1, &list);
		}



		/////////////////////////////////////////////////////////////
		bool DX12CommandQueue::Reset() const
		{
			if (FAILED(m_Allocator->Reset()))
				return false;

			if (FAILED(m_List->Reset(m_Allocator, nullptr)))
				return false;

			return true;
		}



		/////////////////////////////////////////////////////////////
		bool DX12CommandQueue::Close() const
		{
			if (FAILED(m_List->Close()))
				return false;

			return true;
		}



		/////////////////////////////////////////////////////////////
		DX12CommandQueue& DX12CommandQueue::operator=(DX12CommandQueue&& other)
		{
			if (this != &other)
			{
				D3DRelease_S(m_Queue);
				D3DRelease_S(m_Allocator);
				D3DRelease_S(m_List);
				D3DRelease_S(m_Fence);

				m_Queue = other.m_Queue;
				m_Allocator = other.m_Allocator;
				m_List = other.m_List;
				m_Fence = other.m_Fence;
				m_CurrentFence = other.m_CurrentFence;

				other.m_Queue = nullptr;
				other.m_Allocator = nullptr;
				other.m_List = nullptr;
				other.m_Fence = nullptr;
				other.m_CurrentFence = 0;
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		ID3D12CommandQueue* DX12CommandQueue::GetCommandQueue() const
		{
			return m_Queue;
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::Create(ID3D12Device* device, const CommanQueueInfo& info)
		{
			//TODO: different types of commandqueues
			D3D12_COMMAND_QUEUE_DESC qDesc = {};
			qDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			qDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			qDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
			qDesc.NodeMask = 0;

			if (FAILED(device->CreateCommandQueue(&qDesc, IID_PPV_ARGS(&m_Queue))))
				return;


			if (FAILED(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_Allocator))))
				return;


			if (FAILED(device->CreateCommandList(qDesc.NodeMask, D3D12_COMMAND_LIST_TYPE_DIRECT,
				m_Allocator, nullptr, IID_PPV_ARGS(&m_List))))
				return;


			CreateFence(device);
			Close();
			return;
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::CreateFence(ID3D12Device* device)
		{
			if (FAILED(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence))))
			{
				return;
			}
		}
	}
}