#include "..\..\Include\DX12\DX12DescriptorHeap.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12DescriptorHeap::DX12DescriptorHeap()
			: m_Heap(nullptr),
			m_Count(0),
			m_DescriptorSize(0)
		{
		}



		/////////////////////////////////////////////////////////////
		DX12DescriptorHeap::DX12DescriptorHeap(ID3D12Device* pDevice, const std::string& name, D3D12_DESCRIPTOR_HEAP_TYPE type,
			int32 num, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
			: m_Heap(nullptr),
			m_Count(0),
			m_DescriptorSize(0)
		{
			Create(pDevice, name, type, num, flags);
		}



		/////////////////////////////////////////////////////////////
		DX12DescriptorHeap::DX12DescriptorHeap(DX12DescriptorHeap&& other)
			: m_Heap(other.m_Heap),
			m_Count(other.m_Count),
			m_DescriptorSize(other.m_DescriptorSize)
		{
			other.m_Heap = nullptr;
			other.m_Count = 0;
			other.m_DescriptorSize = 0;
		}



		/////////////////////////////////////////////////////////////
		DX12DescriptorHeap::~DX12DescriptorHeap()
		{
			D3DRelease_S(m_Heap);
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

				m_Heap = other.m_Heap;
				m_Count = other.m_Count;
				m_DescriptorSize = other.m_DescriptorSize;

				other.m_Heap = nullptr;
				other.m_Count = 0;
				other.m_DescriptorSize = 0;
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		void DX12DescriptorHeap::Create(ID3D12Device* pDevice, const std::string& name, D3D12_DESCRIPTOR_HEAP_TYPE type,
			int32 num, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
		{
			D3D12_DESCRIPTOR_HEAP_DESC desc = {};
			desc.Flags = flags;
			desc.NodeMask = 0;
			desc.NumDescriptors = num;
			desc.Type = type;

			if (SUCCEEDED(pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_Heap))))
			{
				D3D12SetName(m_Heap, name);

				m_Count = 0;
				m_DescriptorSize = pDevice->GetDescriptorHandleIncrementSize(type);
			}
		}
	}
}