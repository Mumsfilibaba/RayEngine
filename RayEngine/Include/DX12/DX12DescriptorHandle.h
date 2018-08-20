#pragma once

#include "DX12Common.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		struct DX12DescriptorHandle
		{
			int32 DescriptorHeapIndex = 0;
			D3D12_GPU_DESCRIPTOR_HANDLE GpuDescriptor = { 0 };
			D3D12_CPU_DESCRIPTOR_HANDLE CpuDescriptor = { 0 };
			D3D12_GPU_VIRTUAL_ADDRESS GpuResourceAdress = 0;
		};
	}
}

#endif