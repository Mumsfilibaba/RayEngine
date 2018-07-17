#include "..\..\Include\DX12\DX12Texture.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Texture::DX12Texture(ID3D12Device* device, const TextureInfo& info)
			: m_Resource(nullptr)
		{
			Create(device, info);
		}




		/////////////////////////////////////////////////////////////
		DX12Texture::DX12Texture(ID3D12Resource* resource)
			: m_Resource(nullptr)
		{
			Create(resource);
		}



		/////////////////////////////////////////////////////////////
		DX12Texture::DX12Texture(DX12Texture&& other)
			: m_Resource(other.m_Resource)
		{
			other.m_Resource = nullptr;
		}



		/////////////////////////////////////////////////////////////
		DX12Texture::~DX12Texture()
		{
			D3DRelease_S(m_Resource);
		}



		/////////////////////////////////////////////////////////////
		ID3D12Resource* DX12Texture::GetResource() const
		{
			return m_Resource;
		}



		/////////////////////////////////////////////////////////////
		DX12Texture& DX12Texture::operator=(DX12Texture&& other)
		{
			if (this != &other)
			{
				D3DRelease_S(m_Resource);

				m_Resource = other.m_Resource;

				other.m_Resource = nullptr;
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		void DX12Texture::Create(ID3D12Device* device, const TextureInfo& info)
		{
			D3D12_RESOURCE_DESC dbDesc = {};
			memset(&dbDesc, 0, sizeof(D3D12_RESOURCE_DESC));

			dbDesc.Width = info.Width;
			dbDesc.Height = info.Height;

			if (info.TextureType == TEXTURETYPE_1D)
				dbDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
			else if (info.TextureType == TEXTURETYPE_2D)
				dbDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			else if (info.TextureType == TEXTURETYPE_3D)
				dbDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;

			dbDesc.DepthOrArraySize = info.DepthOrArraySize;
			dbDesc.SampleDesc.Count = info.SampleCount;
			dbDesc.SampleDesc.Quality = 0;

			dbDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			dbDesc.Alignment = 0;

			if (info.Flags & TEXTUREFLAGS_TEXTURE)
				dbDesc.Flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
			if (info.Flags & TEXTUREFLAGS_RENDERTARGET)
				dbDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
			if (info.Flags & TEXTUREFLAGS_DEPTHBUFFER)
				dbDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

			dbDesc.MipLevels = info.MipLevels;

			dbDesc.Format = DXGI_FORMAT_D16_UNORM;
		}



		/////////////////////////////////////////////////////////////
		void DX12Texture::Create(ID3D12Resource* resource)
		{
			resource->AddRef();
			m_Resource = resource;
		}
	}
}