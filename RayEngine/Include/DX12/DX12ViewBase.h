#pragma once

#include "DX12DescriptorHeap.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12ViewBase
		{
		public:
			DX12ViewBase(const DX12ViewBase& other) = delete;
			DX12ViewBase& operator=(const DX12ViewBase& other) = delete;

		public:
			DX12ViewBase(const DX12DescriptorHeap* pHeap);
			DX12ViewBase(DX12ViewBase&& other);
			~DX12ViewBase();

			D3D12_CPU_DESCRIPTOR_HANDLE GetD3D12CpuDescriptorHandle() const;

			DX12ViewBase& operator=(DX12ViewBase&& other);

		protected:
			D3D12_CPU_DESCRIPTOR_HANDLE m_ViewHandle;
		};
	}
}