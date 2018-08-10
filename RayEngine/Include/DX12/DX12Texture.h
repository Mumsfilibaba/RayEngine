#pragma once

#include "..\Graphics\ITexture.h"
#include "..\Graphics\IDevice.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12Resource.h"
#include "DX12Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12Texture final : public ITexture, public DX12Resource
		{
		public:
			DX12Texture(const DX12Texture& other) = delete;
			DX12Texture& operator=(const DX12Texture& other) = delete;
			DX12Texture(DX12Texture&& other) = delete;
			DX12Texture& operator=(DX12Texture&& other) = delete;

		public:
			DX12Texture(IDevice* pDevice, const ResourceData* const pInitialData, const TextureInfo& info);
			DX12Texture(IDevice* pDevice, ID3D12Resource* pResource);
			~DX12Texture();
			
			RESOURCE_STATE GetResourceState() const override final;
			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const ResourceData* const pInitialData, const TextureInfo& info);

		private:
			DX12Device* m_Device;
		};
	}
}

#endif