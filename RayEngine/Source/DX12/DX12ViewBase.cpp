#include "..\..\Include\DX12\DX12ViewBase.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12ViewBase::DX12ViewBase(const DX12DescriptorHeap* pHeap)
			: m_ViewHandle()
		{
			m_ViewHandle = pHeap->GetNext();
		}



		/////////////////////////////////////////////////////////////
		DX12ViewBase::DX12ViewBase(DX12ViewBase&& other)
			: m_ViewHandle(other.m_ViewHandle)
		{
			other.m_ViewHandle.ptr = 0;
		}



		/////////////////////////////////////////////////////////////
		DX12ViewBase::~DX12ViewBase()
		{
		}



		/////////////////////////////////////////////////////////////
		D3D12_CPU_DESCRIPTOR_HANDLE DX12ViewBase::GetD3D12CpuDescriptorHandle() const
		{
			return m_ViewHandle;
		}



		/////////////////////////////////////////////////////////////
		DX12ViewBase& DX12ViewBase::operator=(DX12ViewBase&& other)
		{
			if (this != &other)
			{
				m_ViewHandle = other.m_ViewHandle;

				other.m_ViewHandle.ptr = 0;
			}

			return *this;
		}
	}
}