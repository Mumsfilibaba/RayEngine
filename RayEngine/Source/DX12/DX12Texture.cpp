#include "..\..\Include\DX12\DX12Texture.h"
#include "..\..\Include\DX12\DX12Device.h"
#include <utility>

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Texture::DX12Texture(const IDevice* pDevice, const TextureInfo& info)
			: DX12Resource()
		{
			Create(pDevice, info);
		}




		/////////////////////////////////////////////////////////////
		DX12Texture::DX12Texture(ID3D12Resource* pResource)
			: DX12Resource(pResource)
		{
		}



		/////////////////////////////////////////////////////////////
		DX12Texture::DX12Texture(DX12Texture&& other)
			: DX12Resource(std::move(other))
		{
		}



		/////////////////////////////////////////////////////////////
		DX12Texture::~DX12Texture()
		{
		}



		/////////////////////////////////////////////////////////////
		RESOURCE_STATE DX12Texture::GetResourceState() const
		{
			return DXToREResourceState(GetD3D12State());
		}



		/////////////////////////////////////////////////////////////
		DX12Texture& DX12Texture::operator=(DX12Texture&& other)
		{
			DX12Resource::operator=(std::move(other));
			return *this;
		}



		/////////////////////////////////////////////////////////////
		void DX12Texture::Create(const IDevice* pDevice, const TextureInfo& info)
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
			desc.Width = info.Width;
			desc.Height = info.Height;
			desc.DepthOrArraySize = info.DepthOrArraySize;
			desc.SampleDesc.Count = info.SampleCount;
			desc.SampleDesc.Quality = 0;
			desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			desc.Flags = flags;
			desc.MipLevels = info.MipLevels;
			desc.Format = format;

			if (info.Type == TEXTURE_TYPE_1D)
			{
				desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
				desc.Height = 1;
			}
			else if (info.Type == TEXTURE_TYPE_2D)
			{
				desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			}
			else if (info.Type == TEXTURE_TYPE_3D)
			{
				desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
			}


			ID3D12Device* pD3D12Device = reinterpret_cast<const DX12Device*>(pDevice)->GetD3D12Device();
			DX12Resource::Create(pD3D12Device, info.Name, &clearValue, desc, D3D12_RESOURCE_STATE_COMMON, info.Usage, info.CpuAccess);
		}
	}
}