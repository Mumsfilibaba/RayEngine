#include "..\..\Include\DX12\DX12CommandQueue.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12CommandQueue::DX12CommandQueue(ID3D12Device* device, const CommanQueueInfo& info)
			: m_Queue(nullptr),
			m_Allocator(nullptr),
			m_List(nullptr)
		{
			Create(device, info);
		}



		/////////////////////////////////////////////////////////////
		DX12CommandQueue::DX12CommandQueue(DX12CommandQueue&& other)
			: m_Queue(other.m_Queue),
			m_Allocator(other.m_Allocator),
			m_List(other.m_List)
		{
			other.m_Queue = nullptr;
			other.m_Allocator = nullptr;
			other.m_List = nullptr;
		}



		/////////////////////////////////////////////////////////////
		DX12CommandQueue::~DX12CommandQueue()
		{
			D3DRelease_S(m_Queue);
			D3DRelease_S(m_Allocator);
			D3DRelease_S(m_List);
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::Close() const
		{
			m_List->Close();
		}



		/////////////////////////////////////////////////////////////
		DX12CommandQueue& DX12CommandQueue::operator=(DX12CommandQueue&& other)
		{
			if (this != &other)
			{
				m_Queue = other.m_Queue;
				m_Allocator = other.m_Allocator;
				m_List = other.m_List;

				other.m_Queue = nullptr;
				other.m_Allocator = nullptr;
				other.m_List = nullptr;
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


			Close();
			return;
		}
	}
}