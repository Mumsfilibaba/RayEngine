#include "..\..\Include\DX12\DX12DynamicUploadHeap.h"
#include "..\..\Include\DX12\DX12Device.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12DynamicUploadHeap::DX12DynamicUploadHeap(IDevice* pDevice, uint32 alignment, uint32 sizeInBytes)
			: m_Device(nullptr),
			m_Resource(nullptr),
			m_Heap(nullptr),
			m_SizeInBytes(0),
			m_State(D3D12_RESOURCE_STATE_COMMON)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());

			Create(pDevice, alignment, sizeInBytes);
		}



		/////////////////////////////////////////////////////////////
		DX12DynamicUploadHeap::~DX12DynamicUploadHeap()
		{
			D3DRelease_S(m_Heap);
			D3DRelease_S(m_Resource);
			if (m_Device != nullptr)
			{
				m_Device->Release();
				m_Device = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		void DX12DynamicUploadHeap::SetData(const void* pData, int32 sizeInBytes)
		{
			//TODO: If sizeInBytes > size reallocate resource and heap if necessary

			void* pGpuData = nullptr;

			D3D12_RANGE range = { 0, 0 };
			m_Resource->Map(0, &range, &pGpuData);
			memcpy(pGpuData, pData, sizeInBytes);
			m_Resource->Unmap(0, nullptr);
		}



		/////////////////////////////////////////////////////////////
		IDevice* DX12DynamicUploadHeap::GetDevice() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		ID3D12Heap* DX12DynamicUploadHeap::GetD3D12Heap() const
		{
			return m_Heap;
		}



		/////////////////////////////////////////////////////////////
		ID3D12Resource* DX12DynamicUploadHeap::GetD3D12Resource() const
		{
			return m_Resource;
		}



		/////////////////////////////////////////////////////////////
		void DX12DynamicUploadHeap::Create(IDevice* pDevice, uint32 alignment, uint32 sizeInBytes)
		{
			using namespace System;

			D3D12_HEAP_DESC hDesc = {};
			hDesc.Flags = D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS;
			hDesc.Alignment = alignment;
			hDesc.SizeInBytes = sizeInBytes * 4;
			hDesc.Properties.Type = D3D12_HEAP_TYPE_UPLOAD;
			hDesc.Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			hDesc.Properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			hDesc.Properties.CreationNodeMask = 1;
			hDesc.Properties.VisibleNodeMask = 1;

			ID3D12Device* pD3D12Device = reinterpret_cast<DX12Device*>(pDevice)->GetD3D12Device();
			HRESULT hr = pD3D12Device->CreateHeap(&hDesc, IID_PPV_ARGS(&m_Heap));
			if (FAILED(hr))
			{
				pDevice->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D12: Could not create Heap." + DXErrorString(hr));
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
			
			hr = pD3D12Device->CreatePlacedResource(m_Heap, 0, &rDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_Resource));
			if (FAILED(hr))
			{
				pDevice->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D12: Could not create Resource." + DXErrorString(hr));
				return;
			}


			m_SizeInBytes = sizeInBytes;
			return;
		}
	}
}

#endif