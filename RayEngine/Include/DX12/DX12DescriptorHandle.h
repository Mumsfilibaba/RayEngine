#pragma once

#include "DX12Common.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		struct DX12DescriptorHandle
		{
			int32 Index = 0;
			D3D12_CPU_DESCRIPTOR_HANDLE Cpu = { 0 };
			D3D12_GPU_DESCRIPTOR_HANDLE Gpu = { 0 };
		};
	}
}

#endif