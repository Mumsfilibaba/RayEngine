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
#include "DX12Resource.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX12Device;



		/////////////////////////////////////////////////////////////
		class DX12DynamicUploadHeap final : public DX12Resource, public IDeviceObject
		{
		public:
			DX12DynamicUploadHeap(const DX12DynamicUploadHeap& other) = delete;
			DX12DynamicUploadHeap& operator=(const DX12DynamicUploadHeap& other) = delete;
			DX12DynamicUploadHeap(DX12DynamicUploadHeap&& other) = delete;
			DX12DynamicUploadHeap& operator=(DX12DynamicUploadHeap&& other) = delete;

		public:
			DX12DynamicUploadHeap(IDevice* pDevice, const std::string& name, uint32 alignment, uint32 sizeInBytes);
			~DX12DynamicUploadHeap();

			void SetData(const void* pData, int32 size);

			ID3D12Heap* GetD3D12Heap() const;

			void SetName(const std::string& name) override final;
			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const std::string& name, uint32 alignment, uint32 sizeInBytes);

		private:
			DX12Device* m_Device;
			ID3D12Heap* m_Heap;
			int32 m_SizeInBytes;
		};
	}
}

#endif