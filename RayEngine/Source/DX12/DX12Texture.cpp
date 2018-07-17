#include "..\..\Include\DX12\DX12Texture.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Texture::DX12Texture(ID3D12Device* device, const TextureInfo& info)
			: m_Resource(nullptr)
		{
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
		}



		/////////////////////////////////////////////////////////////
		void DX12Texture::Create(ID3D12Resource* resource)
		{
		}
	}
}