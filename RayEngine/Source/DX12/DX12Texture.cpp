#include "..\..\Include\DX12\DX12Texture.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Texture::DX12Texture(ID3D12Resource* resource)
			: m_Resource(nullptr)
		{
			Create(resource);
		}



		/////////////////////////////////////////////////////////////
		DX12Texture::~DX12Texture()
		{
			D3DRelease_S(m_Resource);
		}
	}
}