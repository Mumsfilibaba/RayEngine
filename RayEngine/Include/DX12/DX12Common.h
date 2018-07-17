#pragma once

#include "..\Defines.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\Types.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl\client.h>

#define D3DRelease(IObject) IObject->Release(); IObject = nullptr
#define D3DRelease_S(IObject) if (IObject != nullptr) { D3DRelease(IObject); }

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		struct DX12DescriptorHeap
		{
			ID3D12DescriptorHeap* Heap = nullptr;
			mutable int32 Count = 0;
			int32 DescriptorSize = 0;

			inline D3D12_CPU_DESCRIPTOR_HANDLE GetNext() const
			{
				D3D12_CPU_DESCRIPTOR_HANDLE next = Heap->GetCPUDescriptorHandleForHeapStart();
				next.ptr += Count * DescriptorSize;

				Count++;
				return next;
			}
		};
	}
}

#endif