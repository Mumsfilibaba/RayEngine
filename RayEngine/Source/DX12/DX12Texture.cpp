#include "..\..\Include\DX12\DX12Texture.h"
#include <utility>

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Texture::DX12Texture(ID3D12Device* device, const TextureInfo& info)
			: m_Resource()
		{
			Create(device, info);
		}




		/////////////////////////////////////////////////////////////
		DX12Texture::DX12Texture(ID3D12Resource* resource)
			: m_Resource()
		{
			Create(resource);
		}



		/////////////////////////////////////////////////////////////
		DX12Texture::DX12Texture(DX12Texture&& other)
			: m_Resource()
		{
			m_Resource = std::move(other.m_Resource);
		}



		/////////////////////////////////////////////////////////////
		DX12Texture::~DX12Texture()
		{
		}



		/////////////////////////////////////////////////////////////
		ID3D12Resource* DX12Texture::GetResource() const
		{
			return m_Resource.GetResource();
		}



		/////////////////////////////////////////////////////////////
		DX12Texture& DX12Texture::operator=(DX12Texture&& other)
		{
			if (this != &other)
			{
				m_Resource = std::move(other.m_Resource);
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		void DX12Texture::Create(ID3D12Device* device, const TextureInfo& info)
		{
			D3D12_RESOURCE_FLAGS flags;
			if (info.Flags == TEXTUREFLAGS_RENDERTARGET)
				flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
			if (info.Flags == TEXTUREFLAGS_DEPTHBUFFER)
				flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;


			DXGI_FORMAT format = ReToDXFormat(info.Format);


			D3D12_CLEAR_VALUE clearValue;
			clearValue.Format = DXGI_FORMAT_D16_UNORM;
			clearValue.DepthStencil.Depth = 1.0f;
			clearValue.DepthStencil.Stencil = 0;


			if (info.TextureType == TEXTURETYPE_1D)
			{
				m_Resource = DX12Resource::CreateTexture1D(device, info.Width, info.DepthOrArraySize, 
					info.SampleCount, 0, flags, info.MipLevels, format, info.Usage);
			}
			else if (info.TextureType == TEXTURETYPE_2D)
			{
				m_Resource = DX12Resource::CreateTexture2D(device, info.Width, info.Height, info.DepthOrArraySize,
					info.SampleCount, 0, flags, info.MipLevels, format, info.Usage);
			}
			else if (info.TextureType == TEXTURETYPE_3D)
			{
				m_Resource = DX12Resource::CreateTexture3D(device, info.Width, info.Height, info.DepthOrArraySize,
					info.SampleCount, 0, flags, info.MipLevels, format, info.Usage);
			}
		}



		/////////////////////////////////////////////////////////////
		void DX12Texture::Create(ID3D12Resource* resource)
		{
			resource->AddRef();
			m_Resource = DX12Resource(resource);
		}
	}
}