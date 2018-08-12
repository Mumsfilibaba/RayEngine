#pragma once

#include "..\..\Include\Graphics\ITexture.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX11Device;



		/////////////////////////////////////////////////////////////
		class DX11Texture final : public ITexture
		{
		public:
			DX11Texture(const DX11Texture& other) = delete;
			DX11Texture& operator=(const DX11Texture& other) = delete;
			DX11Texture(DX11Texture&& other) = delete;
			DX11Texture& operator=(DX11Texture&& other) = delete;

		public:
			DX11Texture(IDevice* pDevice, const ResourceData* const pInitialData, const TextureInfo& info);
			DX11Texture(IDevice* pDevice, ID3D11Texture2D* pResource);
			~DX11Texture();

			ID3D11Texture1D* GetD3D11Texture1D() const;
			ID3D11Texture2D* GetD3D11Texture2D() const;
			ID3D11Texture3D* GetD3D11Texture3D() const;

			RESOURCE_STATE GetResourceState() const override final;
			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const ResourceData* const pInitialData, const TextureInfo& info);

		private:
			DX11Device* m_Device;
			TEXTURE_TYPE m_Type;
			union
			{
				ID3D11Texture1D* m_Texture1D;
				ID3D11Texture2D* m_Texture2D;
				ID3D11Texture3D* m_Texture3D;
			};
		};
	}
}

#endif