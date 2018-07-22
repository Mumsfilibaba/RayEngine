#pragma once

#include "..\Graphics\ITexture.h"
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
			DX12Texture(ID3D12Device* device, const TextureInfo& info);
			DX12Texture(ID3D12Resource* resource);
			DX12Texture(DX12Texture&& other);
			~DX12Texture();

			ID3D12Resource* GetResource() const;

			DX12Texture& operator=(DX12Texture&& other);

		private:
			void Create(ID3D12Device* device, const TextureInfo& info);
			void Create(ID3D12Resource* resource);

		private:
			DX12Resource m_Resource;
		};
	}
}