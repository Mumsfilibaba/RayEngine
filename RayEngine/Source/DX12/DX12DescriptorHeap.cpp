#include "..\..\Include\DX12\DX12DescriptorHeap.h"
#include "..\..\Include\DX12\DX12Device.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12DescriptorHeap::DX12DescriptorHeap()
			: m_Device(nullptr),
			m_Heap(nullptr),
			m_Count(0),
			m_DescriptorSize(0),
			m_ReferenceCount(0)
		{
		}



		/////////////////////////////////////////////////////////////
		DX12DescriptorHeap::DX12DescriptorHeap(IDevice* pDevice, const std::string& name, D3D12_DESCRIPTOR_HEAP_TYPE type,
			int32 num, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
			: m_Device(nullptr),
			m_Heap(nullptr),
			m_Count(0),
			m_DescriptorSize(0),
			m_ReferenceCount(0)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());
			Create(pDevice, name, type, num, flags);
		}



		/////////////////////////////////////////////////////////////
		DX12DescriptorHeap::DX12DescriptorHeap(DX12DescriptorHeap&& other)
			: m_Device(other.m_Device),
			m_Heap(other.m_Heap),
			m_Count(other.m_Count),
			m_DescriptorSize(other.m_DescriptorSize),
			m_ReferenceCount(other.m_ReferenceCount)
		{
			other.m_Device = nullptr;
			other.m_Heap = nullptr;
			other.m_Count = 0;
			other.m_DescriptorSize = 0;
			other.m_ReferenceCount = 0;
		}



		/////////////////////////////////////////////////////////////
		DX12DescriptorHeap::~DX12DescriptorHeap()
		{
			D3DRelease_S(m_Heap);
			if (m_Device != nullptr)
			{
				m_Device->Release();
				m_Device = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		D3D12_CPU_DESCRIPTOR_HANDLE DX12DescriptorHeap::GetNext() const
		{
			D3D12_CPU_DESCRIPTOR_HANDLE next = m_Heap->GetCPUDescriptorHandleForHeapStart();
			next.ptr += m_Count * m_DescriptorSize;

			m_Count++;
			return next;
		}



		/////////////////////////////////////////////////////////////
		DX12DescriptorHeap& DX12DescriptorHeap::operator=(DX12DescriptorHeap&& other)
		{
			if (this != &other)
			{
				D3DRelease_S(m_Heap);
				if (m_Device != nullptr)
				{
					m_Device->Release();
					m_Device = nullptr;
				}


				m_Device = other.m_Device;
				m_Heap = other.m_Heap;
				m_Count = other.m_Count;
				m_DescriptorSize = other.m_DescriptorSize;
				m_ReferenceCount = other.m_ReferenceCount;


				other.m_Device = nullptr;
				other.m_Heap = nullptr;
				other.m_Count = 0;
				other.m_DescriptorSize = 0;
				other.m_ReferenceCount = 0;
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		IReferenceCounter * DX12DescriptorHeap::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX12DescriptorHeap::GetReferenceCount() const
		{
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX12DescriptorHeap::Release() const
		{
			m_ReferenceCount--;
			if (m_ReferenceCount < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////s
		uint32 DX12DescriptorHeap::AddRef()
		{
			m_ReferenceCount++;
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX12DescriptorHeap::Create(IDevice* pDevice, const std::string& name, D3D12_DESCRIPTOR_HEAP_TYPE type,
			int32 num, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
		{
			D3D12_DESCRIPTOR_HEAP_DESC desc = {};
			desc.Flags = flags;
			desc.NodeMask = 0;
			desc.NumDescriptors = num;
			desc.Type = type;


			ID3D12Device* pD3D12Device = reinterpret_cast<DX12Device*>(pDevice)->GetD3D12Device();
			if (SUCCEEDED(pD3D12Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_Heap))))
			{
				D3D12SetName(m_Heap, name);

				m_Count = 0;
				m_DescriptorSize = pD3D12Device->GetDescriptorHandleIncrementSize(type);
			}
		}
	}
}

#endif