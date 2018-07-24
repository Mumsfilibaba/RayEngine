#pragma once

#include "..\Graphics\ITexture.h"
#include "..\Graphics\IDevice.h"
#include "DX12Resource.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12Texture : public ITexture
		{
		public:
			DX12Texture(const DX12Texture& other) = delete;
			DX12Texture& operator=(const DX12Texture& other) = delete;

		public:
			DX12Texture(const IDevice* pDevice, const TextureInfo& info);
			DX12Texture(ID3D12Resource* pResource);
			DX12Texture(DX12Texture&& other);
			~DX12Texture();

			const DX12Resource& GetResource() const;

			DX12Texture& operator=(DX12Texture&& other);

		private:
			void Create(const IDevice* pDevice, const TextureInfo& info);
			void Create(ID3D12Resource* pResource);

		private:
			DX12Resource m_Resource;
		};
	}
}