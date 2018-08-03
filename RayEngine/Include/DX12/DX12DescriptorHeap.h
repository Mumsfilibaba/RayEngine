#pragma once

#include "..\Graphics\IDevice.h"
#include "DX12Common.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		class DX12DescriptorHeap : public RefCounter
		{
		public:
			DX12DescriptorHeap(const DX12DescriptorHeap& other) = delete;
			DX12DescriptorHeap& operator=(const DX12DescriptorHeap& other) = delete;
			DX12DescriptorHeap(DX12DescriptorHeap&& other) = delete;
			DX12DescriptorHeap& operator=(DX12DescriptorHeap&& other) = delete;

		public:
			DX12DescriptorHeap(IDevice* pDevice, const std::string& name, D3D12_DESCRIPTOR_HEAP_TYPE type, int32 num, D3D12_DESCRIPTOR_HEAP_FLAGS flags);
			~DX12DescriptorHeap();

			D3D12_CPU_DESCRIPTOR_HANDLE GetNext() const;

		private:
			void Create(IDevice* pDevice, const std::string& name, D3D12_DESCRIPTOR_HEAP_TYPE type, int32 num, D3D12_DESCRIPTOR_HEAP_FLAGS flags);

		private:
			IDevice* m_Device;
			ID3D12DescriptorHeap* m_Heap;
			int32 m_DescriptorSize;
			mutable int32 m_Count;
		};
	}
}

#endif