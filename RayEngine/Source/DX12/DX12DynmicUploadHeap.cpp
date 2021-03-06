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
#include <DX12/DX12DynamicUploadHeap.h>
#include <DX12/DX12Device.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12DynamicUploadHeap::DX12DynamicUploadHeap(IDevice* pDevice, uint32 alignment, uint32 sizeInBytes)
			: DX12Resource(),
			m_Device(nullptr),
			m_Heap(nullptr),
			m_SizeInBytes(0),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX12Device*>(pDevice);

			Create(alignment, sizeInBytes);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12DynamicUploadHeap::~DX12DynamicUploadHeap()
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DynamicUploadHeap::SetData(const void* pData, int32 sizeInBytes)
		{
			//TODO: If sizeInBytes > size reallocate resource and heap if necessary

			void* pGpuData = nullptr;
			D3D12_RANGE range = { 0, 0 };

			m_Resource->Map(0, &range, &pGpuData);
			memcpy(pGpuData, pData, sizeInBytes);
			m_Resource->Unmap(0, nullptr);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX12DynamicUploadHeap::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX12DynamicUploadHeap::Release()
		{
			CounterType counter = --m_References;
			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DynamicUploadHeap::Create(uint32 alignment, uint32 sizeInBytes)
		{
			D3D12_HEAP_DESC hDesc = {};
			hDesc.Flags = D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS;
			hDesc.Alignment = alignment;
			hDesc.SizeInBytes = sizeInBytes * 4;
			hDesc.Properties.Type = D3D12_HEAP_TYPE_UPLOAD;
			hDesc.Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			hDesc.Properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			hDesc.Properties.CreationNodeMask = 1;
			hDesc.Properties.VisibleNodeMask = 1;

			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			HRESULT hr = pD3D12Device->CreateHeap(&hDesc, IID_PPV_ARGS(&m_Heap));
			if (FAILED(hr))
			{
				LOG_ERROR("D3D12: Could not create Heap." + DXErrorString(hr));
				return;
			}


			D3D12_RESOURCE_DESC rDesc = {};
			rDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			rDesc.Width = sizeInBytes;
			rDesc.Height = 1;
			rDesc.DepthOrArraySize = 1;
			rDesc.SampleDesc.Count = 1;
			rDesc.SampleDesc.Quality = 0;
			rDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			rDesc.Alignment = alignment;
			rDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
			rDesc.MipLevels = 1;
			rDesc.Format = DXGI_FORMAT_UNKNOWN;
			
			hr = pD3D12Device->CreatePlacedResource(m_Heap.Get(), 0, &rDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_Resource));
			if (FAILED(hr))
			{
				LOG_ERROR("D3D12: Could not create Resource." + DXErrorString(hr));
			}
			else
			{
				m_SizeInBytes = sizeInBytes;
			}
		}
	}
}

#endif