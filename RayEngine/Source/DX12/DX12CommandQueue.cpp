#include "..\..\Include\DX12\DX12CommandQueue.h"
#include "..\..\Include\DX12\DX12Texture.h"
#include "..\..\Include\DX12\DX12Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12CommandQueue::DX12CommandQueue()
			: m_Queue(nullptr),
			m_Allocator(nullptr),
			m_List(nullptr),
			m_Fence(nullptr),
			m_CurrentFence(0)
		{
		}



		/////////////////////////////////////////////////////////////
		DX12CommandQueue::DX12CommandQueue(const IDevice* pDevice, const CommandQueueInfo& info)
			: m_Queue(nullptr),
			m_Allocator(nullptr),
			m_List(nullptr),
			m_Fence(nullptr),
			m_CurrentFence(0)
		{
			Create(pDevice, info);
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
		void DX12CommandQueue::TransitionResource(const DX12Resource& resource, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to, int32 subresource) const
		{
			D3D12_RESOURCE_BARRIER barrier = {};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

			barrier.Transition.Subresource = subresource;

			barrier.Transition.pResource = resource.GetD3D12Resource();

			barrier.Transition.StateBefore = from;
			barrier.Transition.StateAfter = to;

			m_List->ResourceBarrier(1, &barrier);
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::TransitionResource(const ITexture* pResource, RESOURCE_STATE from, RESOURCE_STATE to, int32 subresource) const
		{
			const DX12Texture* pDX12resource = reinterpret_cast<const DX12Texture*>(pResource);
			TransitionResource(pDX12resource->GetResource(), ReToDXResourceState(from), ReToDXResourceState(to), subresource);
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
			ID3D12CommandList* pList = m_List;
			m_Queue->ExecuteCommandLists(1, &pList);
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
		ID3D12CommandQueue* DX12CommandQueue::GetD3D12CommandQueue() const
		{
			return m_Queue;
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::Create(const IDevice* pDevice, const CommandQueueInfo& info)
		{
			ID3D12Device* pD3D12Device = reinterpret_cast<const DX12Device*>(pDevice)->GetD3D12Device();

			//TODO: different types of commandqueues

			D3D12_COMMAND_QUEUE_DESC qDesc = {};
			qDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			qDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			qDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
			qDesc.NodeMask = 0;

			if (FAILED(pD3D12Device->CreateCommandQueue(&qDesc, IID_PPV_ARGS(&m_Queue))))
				return;
			else
				D3D12SetName(m_Queue, info.Name);


			if (FAILED(pD3D12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_Allocator))))
				return;
			else
				D3D12SetName(m_Queue, info.Name + ": Allocator");


			if (FAILED(pD3D12Device->CreateCommandList(qDesc.NodeMask, D3D12_COMMAND_LIST_TYPE_DIRECT, m_Allocator, nullptr, IID_PPV_ARGS(&m_List))))
				return;
			else
				D3D12SetName(m_Queue, info.Name + ": List");


			if (FAILED(pD3D12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence))))
			{
				return;
			}


			Close();
			return;
		}
	}
}