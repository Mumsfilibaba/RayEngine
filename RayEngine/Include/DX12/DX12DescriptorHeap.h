/*////////////////////////////////////////////////////////////

Copyright 2018 Alexander Dahlin

Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in
compliance with the License. You may obtain a copy of
the License at

http ://www.apache.org/licenses/LICENSE-2.0

THIS SOFTWARE IS PROVIDED "AS IS". MEANING NO WARRANTY
OR SUPPORT IS PROVIDED OF ANY KIND.

In event of any damages, direct or indirect that can
be traced back to the use of this software, shall no
contributor be held liable. This includes computer
failure and or malfunction of any kind.

////////////////////////////////////////////////////////////*/

#pragma once
#include "..\Graphics\IDeviceObject.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12DescriptorHandle.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX12Device;
		class DX12Resource;



		/////////////////////////////////////////////////////////////
		class DX12DescriptorHeap final : public IDeviceObject
		{
		public:
			DX12DescriptorHeap(const DX12DescriptorHeap& other) = delete;
			DX12DescriptorHeap& operator=(const DX12DescriptorHeap& other) = delete;
			DX12DescriptorHeap(DX12DescriptorHeap&& other) = delete;
			DX12DescriptorHeap& operator=(DX12DescriptorHeap&& other) = delete;

		public:
			DX12DescriptorHeap(IDevice* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE type, int32 num, D3D12_DESCRIPTOR_HEAP_FLAGS flags);
			DX12DescriptorHeap(IDevice* pDevice, const std::string& name, D3D12_DESCRIPTOR_HEAP_TYPE type, int32 num, D3D12_DESCRIPTOR_HEAP_FLAGS flags);
			~DX12DescriptorHeap();

			int32 GetDescriptorsLeft() const;
			DX12DescriptorHandle GetNext(const DX12Resource* pResource) const;

			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const std::string& name, D3D12_DESCRIPTOR_HEAP_TYPE type, int32 num, D3D12_DESCRIPTOR_HEAP_FLAGS flags);

		private:
			DX12Device* m_Device;
			ID3D12DescriptorHeap* m_Heap;
			int32 m_DescriptorSize;
			int32 m_Count;
			mutable int32 m_UsedCount;
		};
	}
}

#endif