#include "..\..\Include\DX12\DX12DynamicUploadHeap.h"

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
			m_ReferenceCount(0)
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
		void DX12DynamicUploadHeap::SetData(void* pData, int32 size)
		{
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
		IReferenceCounter* DX12DynamicUploadHeap::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX12DynamicUploadHeap::GetReferenceCount() const
		{
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX12DynamicUploadHeap::Release() const
		{
			m_ReferenceCount--;
			if (m_ReferenceCount < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX12DynamicUploadHeap::AddRef()
		{
			m_ReferenceCount++;
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX12DynamicUploadHeap::Create(IDevice* pDevice, uint32 alignment, uint32 sizeInBytes)
		{
			using namespace System;

			D3D12_HEAP_DESC hDesc = {};
			hDesc.Alignment = alignment;
			hDesc.Flags = D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS;
			hDesc.Properties = ;
			hDesc.SizeInBytes = size;

			ID3D12Device* pD3D12Device = reinterpret_cast<DX12Device*>(pDevice)->GetD3D12Device();
			if (FAILED(pD3D12Device->CreateHeap(&hDesc, IID_PPV_ARGS(&m_Heap))))
			{
				pDevice->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D12: Could not create Heap");
				return;
			}
		}
	}
}

#endif