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

			IDevice* GetDevice() const;
			ID3D12Heap* GetD3D12Heap() const;

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