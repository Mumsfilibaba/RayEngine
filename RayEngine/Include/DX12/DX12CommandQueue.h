#pragma once

#include <RayEngine.h>
#include <Interfaces/IObject.h>

namespace RayEngine
{
	namespace Graphics
	{
		class DX12Device;

		class DX12CommandQueue final : public IObject
		{
			RE_UNIQUE_OBJECT(DX12CommandQueue);
		
		public:
			DX12CommandQueue(ID3D12CommandQueue* pQueue, ID3D12Fence* pFence);
			~DX12CommandQueue();

			CounterType AddRef() override final;

			CounterType Release() override final;

		private:
			Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_Queue;
			CounterType m_References;
		};
	}
}