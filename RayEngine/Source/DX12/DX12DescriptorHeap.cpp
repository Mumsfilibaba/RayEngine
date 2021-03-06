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

#include <RayEngine.h>

#if defined(RE_PLATFORM_WINDOWS)
#include <DX12/DX12DescriptorHeap.h>
#include <DX12/DX12Device.h>
#include <DX12/DX12Resource.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12DescriptorHeap::DX12DescriptorHeap(IDevice* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAGS flags, int32 num)
			: m_Device(nullptr),
			m_Heap(nullptr),
			m_Count(0),
			m_UsedCount(0),
			m_DescriptorSize(0),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX12Device*>(pDevice);

			Create(type, flags, num);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12DescriptorHeap::~DX12DescriptorHeap()
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12DescriptorHandle DX12DescriptorHeap::GetNext() const
		{
			DX12DescriptorHandle next = {};
			next.DescriptorHeapIndex = m_UsedCount;

			next.CpuDescriptor = m_Heap->GetCPUDescriptorHandleForHeapStart();
			next.CpuDescriptor.ptr += m_UsedCount * m_DescriptorSize;

			next.GpuDescriptor = m_Heap->GetGPUDescriptorHandleForHeapStart();
			next.GpuDescriptor.ptr += m_UsedCount * m_DescriptorSize;

			m_UsedCount++;
			return next;
		}

	
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX12DescriptorHeap::AddRef()
		{
			return ++m_References;
		}

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX12DescriptorHeap::Release()
		{
			CounterType counter = --m_References;
			if (counter < 1)
				delete this;

			return counter;
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DescriptorHeap::Create(D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAGS flags, int32 num)
		{
			D3D12_DESCRIPTOR_HEAP_DESC desc = {};
			desc.Flags = flags;
			desc.NodeMask = 0;
			desc.NumDescriptors = num;
			desc.Type = type;

			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			HRESULT hr = pD3D12Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_Heap));
			if (FAILED(hr))
			{
				LOG_ERROR("D3D12: Could not create DescriptorHeap. " + DXErrorString(hr));
			}
			else
			{
				m_Count = num;
				m_UsedCount = 0;
				m_DescriptorSize = pD3D12Device->GetDescriptorHandleIncrementSize(type);
			}
		}
	}
}

#endif