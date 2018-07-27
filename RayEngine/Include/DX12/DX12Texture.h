#pragma once

#include "..\Graphics\ITexture.h"
#include "..\Graphics\IDevice.h"
#include "DX12Resource.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12Texture : public ITexture, public DX12Resource
		{
		public:
			DX12Texture(const DX12Texture& other) = delete;
			DX12Texture& operator=(const DX12Texture& other) = delete;

		public:
			DX12Texture(const IDevice* pDevice, const ResourceData* const pInitialData, const TextureInfo& info);
			DX12Texture(ID3D12Resource* pResource);
			DX12Texture(DX12Texture&& other);
			~DX12Texture();

			RESOURCE_STATE GetResourceState() const override final;

			DX12Texture& operator=(DX12Texture&& other);

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void Create(const IDevice* pDevice, const ResourceData* const pInitialData, const TextureInfo& info);

		private:
			mutable uint32 m_ReferenceCount;
		};
	}
}