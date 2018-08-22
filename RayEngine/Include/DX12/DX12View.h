#pragma once

#include "DX12DescriptorHandle.h"
#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX12Device;
		class DX12Resource;



		/////////////////////////////////////////////////////////////
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
			DX12Resource* GetD3D12Resource() const;
			DX12DescriptorHandle GetDX12DescriptorHandle() const;
			D3D12_CPU_DESCRIPTOR_HANDLE GetD3D12CpuDescriptorHandle() const;
			D3D12_GPU_DESCRIPTOR_HANDLE GetD3D12GpuDescriptorHandle() const;

		protected:
			DX12Resource* m_Resource;
			DX12DescriptorHandle m_View;
		};
	}
}

#endif