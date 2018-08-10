#pragma once

#include "DX12Common.h"
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
			D3D12_CPU_DESCRIPTOR_HANDLE GetD3D12CpuDescriptorHandle() const;

		protected:
			D3D12_CPU_DESCRIPTOR_HANDLE m_View;
		};
	}
}

#endif