#pragma once

#include "DX12Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12Resource
		{
		public:
			DX12Resource();
			DX12Resource(ID3D12Device* device, D3D12_CLEAR_VALUE* clearValue,
				const D3D12_RESOURCE_DESC& desc, RESOURCE_USAGE usage);
			DX12Resource(ID3D12Resource* resource);
			DX12Resource(const DX12Resource& other);
			DX12Resource(DX12Resource&& other);
			~DX12Resource();

			void GetDesc(D3D12_RESOURCE_DESC& desc);

			ID3D12Resource* GetResource() const;
			D3D12_RESOURCE_STATES GetState() const;

			DX12Resource& operator=(const DX12Resource& other);
			DX12Resource& operator=(DX12Resource&& other);

		private:
			ID3D12Resource* m_Resource;
			ID3D12Resource* m_UploadResource;
			mutable D3D12_RESOURCE_STATES m_State;

		private:
			void Create(ID3D12Device* device, D3D12_CLEAR_VALUE* clearValue,
				const D3D12_RESOURCE_DESC& desc, RESOURCE_USAGE usage);

		public:
			static DX12Resource CreateTexture1D(ID3D12Device* device, uint64 width, uint32 arraySize, 
				uint32 sampleCount, uint32 sampleQuality, D3D12_RESOURCE_FLAGS flags, uint16 miplevels, 
				DXGI_FORMAT format, RESOURCE_USAGE usage);
			static DX12Resource CreateTexture2D(ID3D12Device* device, uint64 width, uint32 height,
				uint32 arraySize, uint32 sampleCount, uint32 sampleQuality, D3D12_RESOURCE_FLAGS flags,
				uint16 miplevels, DXGI_FORMAT format, RESOURCE_USAGE usage);
			static DX12Resource CreateTexture3D(ID3D12Device* device, uint64 width, uint32 height,
				uint32 depth, uint32 sampleCount, uint32 sampleQuality, D3D12_RESOURCE_FLAGS flags,
				uint16 miplevels, DXGI_FORMAT format, RESOURCE_USAGE usage);
			static DX12Resource CreateBuffer(ID3D12Device* device, uint32 count, uint32 stride,
				D3D12_RESOURCE_FLAGS flags, RESOURCE_USAGE usage);

		private:
			static DX12Resource Create(ID3D12Device* device, D3D12_CLEAR_VALUE* clearValue, D3D12_RESOURCE_DIMENSION dimension,
				uint64 width, uint32 height, uint16 depthOrArraySize, uint32 sampleCount, uint32 sampleQuality, 
				D3D12_TEXTURE_LAYOUT layout, uint32 alignment, D3D12_RESOURCE_FLAGS flags, uint16 miplevels, 
				DXGI_FORMAT format, RESOURCE_USAGE usage);
		};
	}
}