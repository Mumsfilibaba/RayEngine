#include "..\..\Include\DX12\DX12Fence.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Fence::DX12Fence(ID3D12Device* device)
			: m_Fence(nullptr),
			m_CurrentFence(0)
		{
			Create(device);
		}



		/////////////////////////////////////////////////////////////
		DX12Fence::DX12Fence(DX12Fence&& other)
			: m_Fence(other.m_Fence),
			m_CurrentFence(other.m_CurrentFence)
		{
			other.m_Fence = nullptr;
			other.m_CurrentFence = 0;
		}



		/////////////////////////////////////////////////////////////
		DX12Fence::~DX12Fence()
		{
			D3DRelease_S(m_Fence);
		}



		/////////////////////////////////////////////////////////////
		DX12Fence& DX12Fence::operator=(DX12Fence&& other)
		{
			if (this != &other)
			{
				m_Fence = other.m_Fence;
				m_CurrentFence = other.m_CurrentFence;

				other.m_Fence = nullptr;
				other.m_CurrentFence = 0;
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		void DX12Fence::Create(ID3D12Device* device)
		{
			if (FAILED(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence))))
				return;

			return;
		}
	}
}