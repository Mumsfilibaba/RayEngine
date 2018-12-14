#include <RayEngine.h>
#include <DX12/DX12CommandQueue.h>

namespace RayEngine
{
	namespace Graphics
	{
		DX12CommandQueue::DX12CommandQueue(ID3D12CommandQueue* pQueue, ID3D12Fence* pFence)
			: m_Queue(nullptr),
			m_Fence(nullptr),
			m_References(0)
		{
			AddRef();
			
			m_Queue = pQueue;
			m_Queue->AddRef();

			m_Fence = pFence;
			m_Fence->AddRef();
		}


		DX12CommandQueue::~DX12CommandQueue()
		{
		}


		CounterType DX12CommandQueue::AddRef()
		{
			return ++m_References;
		}


		CounterType DX12CommandQueue::Release()
		{
			CounterType counter = --m_References;
			if (counter < 1)
				delete this;

			return counter;
		}
	}
}