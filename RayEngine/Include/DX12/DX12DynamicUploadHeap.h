#pragma once

#include "..\Graphics\IDevice.h"
#include "DX12Common.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		class DX12DynamicUploadHeap : public RefCounter
		{
		public:
			DX12DynamicUploadHeap(const DX12DynamicUploadHeap& other) = delete;
			DX12DynamicUploadHeap& operator=(const DX12DynamicUploadHeap& other) = delete;
			DX12DynamicUploadHeap(DX12DynamicUploadHeap&& other) = delete;
			DX12DynamicUploadHeap& operator=(DX12DynamicUploadHeap&& other) = delete;

		public:
			DX12DynamicUploadHeap(IDevice* pDevice, uint32 alignment, uint32 sizeInBytes);
			~DX12DynamicUploadHeap();

			void SetData(const void* pData, int32 size);

			IDevice* GetDevice() const;
			ID3D12Heap* GetD3D12Heap() const;
			ID3D12Resource* GetD3D12Resource() const;

		private:
			void Create(IDevice* pDevice, uint32 alignment, uint32 sizeInBytes);

		private:
			IDevice* m_Device;
			ID3D12Heap* m_Heap;
			ID3D12Resource* m_Resource;
			D3D12_RESOURCE_STATES m_State;
			int32 m_SizeInBytes;
		};
	}
}

#endif