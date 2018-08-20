#pragma once

#include "DX12ViewHandle.h"
#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		class DX12View
		{
		public:
			DX12View(const DX12View& other) = delete;
			DX12View& operator=(const DX12View& other) = delete;
			DX12View(DX12View&& other) = delete;
			DX12View& operator=(DX12View&& other) = delete;

		protected:
			DX12View();
			~DX12View();

		public:
			DX12DescriptorHandle GetViewHandle() const;
			D3D12_CPU_DESCRIPTOR_HANDLE GetD3D12CpuDescriptorHandle() const;
			D3D12_GPU_DESCRIPTOR_HANDLE GetD3D12GpuDescriptorHandle() const;

		protected:
			DX12DescriptorHandle m_View;
		};
	}
}

#endif