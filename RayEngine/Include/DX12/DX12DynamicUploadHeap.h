#pragma once

#include "DX12Device.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		class DX12DynamicUploadHeap : IReferenceCounter
		{
		public:
			DX12DynamicUploadHeap(const DX12DynamicUploadHeap& other) = delete;
			DX12DynamicUploadHeap& operator=(const DX12DynamicUploadHeap& other) = delete;

		public:
			DX12DynamicUploadHeap(IDevice* pDevice, uint32 alignment, uint32 sizeInBytes);
			~DX12DynamicUploadHeap();

			void SetData(void* pData, int32 size);

			IDevice* GetDevice() const;
			ID3D12Heap* GetD3D12Heap() const;
			ID3D12Resource* GetD3D12Resource() const;

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void Create(IDevice* pDevice, uint32 alignment, uint32 sizeInBytes);

		private:
			IDevice* m_Device;
			
			ID3D12Heap* m_Heap;
			ID3D12Resource* m_Resource;
			D3D12_RESOURCE_STATES m_State;

			mutable uint32 m_ReferenceCount;
		};
	}
}

#endif