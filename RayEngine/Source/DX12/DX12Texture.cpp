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
			D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE;
			if (info.Flags == TEXTURE_FLAGS_RENDERTARGET)
				flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
			if (info.Flags == TEXTURE_FLAGS_DEPTHBUFFER)
				flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;


			DXGI_FORMAT format = ReToDXFormat(info.Format);


			D3D12_CLEAR_VALUE clearValue = {};
			clearValue.Format = format;
			clearValue.Color[0] = info.OptimizedColor[0];
			clearValue.Color[1] = info.OptimizedColor[1];
			clearValue.Color[2] = info.OptimizedColor[2];
			clearValue.Color[3] = info.OptimizedColor[3];

			clearValue.DepthStencil.Depth = info.DepthStencil.OptimizedDepth;
			clearValue.DepthStencil.Stencil = info.DepthStencil.OptimizedStencil;


			D3D12_RESOURCE_DESC desc = {};
			if (info.Type == TEXTURE_TYPE_1D)
			{
				desc = DX12Resource::CreateDescTexture1D(info.Width, info.DepthOrArraySize, info.SampleCount, 0,
					flags, info.MipLevels, format);
			}
			else if (info.Type == TEXTURE_TYPE_2D)
			{
				desc = DX12Resource::CreateDescTexture2D(info.Width, info.Height, info.DepthOrArraySize,
					info.SampleCount, 0, flags, info.MipLevels, format);
			}
			else if (info.Type == TEXTURE_TYPE_3D)
			{
				desc = DX12Resource::CreateDescTexture3D(info.Width, info.Height, info.DepthOrArraySize, 
					info.SampleCount, 0, flags, info.MipLevels, format);
			}


			m_Resource = DX12Resource(device, info.Name, &clearValue, desc, D3D12_RESOURCE_STATE_COMMON, info.Usage, info.CpuAccess);
		}



		/////////////////////////////////////////////////////////////
		void DX12Texture::Create(ID3D12Resource* resource)
		{
			resource->AddRef();
			m_Resource = DX12Resource(resource);
		}
	}
}