#pragma once

#include "DX12Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12DescriptorHeap
		{
		public:
			DX12DescriptorHeap(const DX12DescriptorHeap& other) = delete;
			DX12DescriptorHeap& operator=(const DX12DescriptorHeap& other) = delete;

		public:
			DX12DescriptorHeap();
			DX12DescriptorHeap(ID3D12Device* pDevice, const std::string& name, D3D12_DESCRIPTOR_HEAP_TYPE type, int32 num, D3D12_DESCRIPTOR_HEAP_FLAGS flags);
			DX12DescriptorHeap(DX12DescriptorHeap&& other);
			~DX12DescriptorHeap();

			D3D12_CPU_DESCRIPTOR_HANDLE GetNext() const;

			DX12DescriptorHeap& operator=(DX12DescriptorHeap&& other);

		private:
			void Create(ID3D12Device* pDevice, const std::string& name, D3D12_DESCRIPTOR_HEAP_TYPE type, int32 num, D3D12_DESCRIPTOR_HEAP_FLAGS flags);

		private:
			ID3D12DescriptorHeap* m_Heap;
			mutable int32 m_Count;
			int32 m_DescriptorSize;
		};
	}
}