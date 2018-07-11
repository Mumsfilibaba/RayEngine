#pragma once

#include "..\Graphics\IFence.h"
#include "DX12Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12Fence : public IFence
		{
		public:
			DX12Fence(const DX12Fence& other) = delete;
			DX12Fence& operator=(const DX12Fence& other) = delete;

		public:
			DX12Fence(ID3D12Device* device);
			DX12Fence(DX12Fence&& other);
			~DX12Fence();

			DX12Fence& operator=(DX12Fence&& other);

		private:
			void Create(ID3D12Device* device);

		private:
			uint64 m_CurrentFence;
			ID3D12Fence* m_Fence;
		};
	}
}