#pragma once

#include "..\Graphics\ITexture.h"
#include "..\Graphics\IDevice.h"
#include "DX12Common.h"

#if defined(RE_PLATFORM_WINDOWS)

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
			DX12Texture(IDevice* pDevice, const ResourceData* const pInitialData, const TextureInfo& info);
			DX12Texture(IDevice* pDevice, ID3D12Resource* pResource);
			DX12Texture(DX12Texture&& other);
			~DX12Texture();

			DX12Texture& operator=(DX12Texture&& other);
			
			ID3D12Resource* GetD3D12Resource() const;
			D3D12_RESOURCE_STATES GetD3D12State() const;
			void SetD3D12State(D3D12_RESOURCE_STATES state) const;

			RESOURCE_STATE GetResourceState() const override final;
			IDevice* GetDevice() const override final;

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void Create(IDevice* pDevice, const ResourceData* const pInitialData, const TextureInfo& info);

		private:
			IDevice* m_Device;

			ID3D12Resource* m_Resource;
			mutable D3D12_RESOURCE_STATES m_State;

			mutable uint32 m_ReferenceCount;
		};
	}
}

#endif