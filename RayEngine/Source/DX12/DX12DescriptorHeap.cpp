/*////////////////////////////////////////////////////////////

Copyright 2018 Alexander Dahlin

Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in
compliance with the License. You may obtain a copy of
the License at

http ://www.apache.org/licenses/LICENSE-2.0

THIS SOFTWARE IS PROVIDED "AS IS". MEANING NO WARRANTY
OR SUPPORT IS PROVIDED OF ANY KIND.

In event of any damages, direct or indirect that can
be traced back to the use of this software, shall no
contributor be held liable. This includes computer
failure and or malfunction of any kind.

////////////////////////////////////////////////////////////*/

#include "..\..\Include\DX12\DX12DescriptorHeap.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12Resource.h"

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
		DX12DescriptorHandle DX12DescriptorHeap::GetNext(const DX12Resource* pResource) const
		{
			DX12DescriptorHandle next = {};
			next.DescriptorHeapIndex = m_UsedCount;

			next.CpuDescriptor = m_Heap->GetCPUDescriptorHandleForHeapStart();
			next.CpuDescriptor.ptr += m_UsedCount * m_DescriptorSize;

			next.GpuDescriptor = m_Heap->GetGPUDescriptorHandleForHeapStart();
			next.GpuDescriptor.ptr += m_UsedCount * m_DescriptorSize;

			if (pResource != nullptr)
				next.GpuResourceAdress = pResource->GetD3D12Resource()->GetGPUVirtualAddress();

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