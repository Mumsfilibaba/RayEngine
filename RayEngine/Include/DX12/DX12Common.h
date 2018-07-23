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
	/////////////////////////////////////////////////////////////
	DXGI_FORMAT ReToDXFormat(FORMAT format);
	D3D12_RESOURCE_STATES ReToDXResourceState(RESOURCE_STATE state);
	


	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		struct DX12DescriptorHeap
		{
		public:
			DX12DescriptorHeap(const DX12DescriptorHeap& other) = delete;
			DX12DescriptorHeap& operator=(const DX12DescriptorHeap& other) = delete;

		public:
			/////////////////////////////////////////////////////////////
			ID3D12DescriptorHeap* Heap = nullptr;
			mutable int32 Count = 0;
			int32 DescriptorSize = 0;
		
		public:
			/////////////////////////////////////////////////////////////
			inline DX12DescriptorHeap()
			{
			}



			/////////////////////////////////////////////////////////////
			inline DX12DescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type,
				int32 num, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
			{
				D3D12_DESCRIPTOR_HEAP_DESC desc = {};
				desc.Flags = flags;
				desc.NodeMask = 0;
				desc.NumDescriptors = num;
				desc.Type = type;

				if (SUCCEEDED(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&Heap))))
				{
					Count = 0;
					DescriptorSize = device->GetDescriptorHandleIncrementSize(type);
				}
			}



			/////////////////////////////////////////////////////////////
			inline DX12DescriptorHeap(DX12DescriptorHeap&& other)
				: Heap(other.Heap),
				Count(other.Count),
				DescriptorSize(other.DescriptorSize)
			{
				other.Heap = nullptr;
				other.Count = 0;
				other.DescriptorSize = 0;
			}



			/////////////////////////////////////////////////////////////
			inline DX12DescriptorHeap& operator=(DX12DescriptorHeap&& other)
			{
				if (this != &other)
				{
					D3DRelease_S(Heap);

					Heap = other.Heap;
					Count = other.Count;
					DescriptorSize = other.DescriptorSize;

					other.Heap = nullptr;
					other.Count = 0;
					other.DescriptorSize = 0;
				}

				return *this;
			}



			/////////////////////////////////////////////////////////////
			inline ~DX12DescriptorHeap()
			{
				D3DRelease_S(Heap);
			}



			/////////////////////////////////////////////////////////////
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