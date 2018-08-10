#include "..\..\Include\DX12\DX12DescriptorHeap.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX12\DX12Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12DescriptorHeap::DX12DescriptorHeap(IDevice* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE type, int32 num, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
			: DX12DescriptorHeap(pDevice, "", type, num, flags)
		{
		}



		/////////////////////////////////////////////////////////////
		DX12DescriptorHeap::DX12DescriptorHeap(IDevice* pDevice, const std::string& name, D3D12_DESCRIPTOR_HEAP_TYPE type, int32 num, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
			: m_Device(nullptr),
			m_Heap(nullptr),
			m_Count(0),
			m_UsedCount(0),
			m_DescriptorSize(0)
		{
			AddRef();
			m_Device = QueryDX12Device(pDevice);

			Create(name, type, num, flags);
		}



		/////////////////////////////////////////////////////////////
		DX12DescriptorHeap::~DX12DescriptorHeap()
		{
			D3DRelease_S(m_Heap);
			
			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		int32 DX12DescriptorHeap::GetDescriptorsLeft() const
		{
			return m_Count - m_UsedCount;
		}



		/////////////////////////////////////////////////////////////
		D3D12_CPU_DESCRIPTOR_HANDLE DX12DescriptorHeap::GetNext() const
		{
			D3D12_CPU_DESCRIPTOR_HANDLE next = m_Heap->GetCPUDescriptorHandleForHeapStart();
			next.ptr += m_UsedCount * m_DescriptorSize;

			m_UsedCount++;
			return next;
		}



		/////////////////////////////////////////////////////////////
		void DX12DescriptorHeap::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryDX12Device(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX12DescriptorHeap::Create(const std::string& name, D3D12_DESCRIPTOR_HEAP_TYPE type, int32 num, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
		{
			using namespace System;

			D3D12_DESCRIPTOR_HEAP_DESC desc = {};
			desc.Flags = flags;
			desc.NodeMask = 0;
			desc.NumDescriptors = num;
			desc.Type = type;


			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			HRESULT hr = pD3D12Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_Heap));
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D12: Could not create DescriptorHeap. " + DXErrorString(hr));
			}
			else
			{
				m_Count = num;
				m_UsedCount = 0;
				m_DescriptorSize = pD3D12Device->GetDescriptorHandleIncrementSize(type);

				D3D12SetName(m_Heap, name);
			}
		}
	}
}

#endif