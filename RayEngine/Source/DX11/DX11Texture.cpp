#include "..\..\Include\DX11\DX11Texture.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11Texture::DX11Texture()
			: m_Device(nullptr),
			m_Type(TEXTURE_TYPE_UNKNOWN),
			m_ReferenceCount(0)
		{
			AddRef();
			m_Texture2D = nullptr;
		}



		/////////////////////////////////////////////////////////////
		DX11Texture::DX11Texture(IDevice* pDevice, const ResourceData* const pInitialData, const TextureInfo& info)
			: m_Device(nullptr),
			m_Type(TEXTURE_TYPE_UNKNOWN),
			m_ReferenceCount(0)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());

			Create(pDevice, pInitialData, info);
		}



		/////////////////////////////////////////////////////////////
		DX11Texture::DX11Texture(IDevice* pDevice, ID3D11Texture2D* pResource)
			: m_Device(nullptr),
			m_Type(TEXTURE_TYPE_2D),
			m_ReferenceCount(0)
		{
			AddRef();

			pResource->AddRef();
			m_Texture2D = pResource;
		}



		/////////////////////////////////////////////////////////////
		DX11Texture::DX11Texture(DX11Texture&& other)
			: m_Device(other.m_Device),
			m_Type(other.m_Type),
			m_ReferenceCount(other.m_ReferenceCount)
		{
			other.m_Device = nullptr;
			other.m_Type = TEXTURE_TYPE_UNKNOWN;
			m_ReferenceCount = 0;

			//Pointers all point to same memory
			m_Texture2D = other.m_Texture2D;
			other.m_Texture2D = nullptr;
		}



		/////////////////////////////////////////////////////////////
		DX11Texture::~DX11Texture()
		{
			if (m_Type == TEXTURE_TYPE_1D) 
			{
				D3DRelease_S(m_Texture1D);
			}
			else if (m_Type == TEXTURE_TYPE_2D)
			{
				D3DRelease_S(m_Texture2D);
			}
			else if (m_Type == TEXTURE_TYPE_3D)
			{
				D3DRelease_S(m_Texture3D);
			}

			if (m_Device != nullptr)
			{
				m_Device->Release();
				m_Device = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		DX11Texture& DX11Texture::operator=(DX11Texture&& other)
		{
			if (this != &other)
			{
				if (m_Type == TEXTURE_TYPE_1D)
				{
					D3DRelease_S(m_Texture1D);
				}
				else if (m_Type == TEXTURE_TYPE_2D)
				{
					D3DRelease_S(m_Texture2D);
				}
				else if (m_Type == TEXTURE_TYPE_3D)
				{
					D3DRelease_S(m_Texture3D);
				}

				if (m_Device != nullptr)
				{
					m_Device->Release();
					m_Device = nullptr;
				}


				m_Device = other.m_Device;
				m_Type = other.m_Type;
				m_ReferenceCount = other.m_ReferenceCount;


				other.m_Device = nullptr;
				other.m_Type = TEXTURE_TYPE_UNKNOWN;
				m_ReferenceCount = 0;

				//Pointers all point to same memory
				m_Texture2D = other.m_Texture2D;
				other.m_Texture2D = nullptr;
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		ID3D11Texture1D* DX11Texture::GetD3D11Texture1D() const
		{
			return m_Texture1D;
		}



		/////////////////////////////////////////////////////////////s
		ID3D11Texture2D* DX11Texture::GetD3D11Texture2D() const
		{
			return m_Texture2D;
		}



		/////////////////////////////////////////////////////////////
		ID3D11Texture3D* DX11Texture::GetD3D11Texture3D() const
		{
			return m_Texture3D;
		}



		/////////////////////////////////////////////////////////////
		RESOURCE_STATE DX11Texture::GetResourceState() const
		{
			return RESOURCE_STATE_UNKNOWN;
		}



		/////////////////////////////////////////////////////////////
		IDevice* DX11Texture::GetDevice() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		IReferenceCounter* DX11Texture::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX11Texture::GetReferenceCount() const
		{
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX11Texture::Release() const
		{
			m_ReferenceCount--;
			if (m_ReferenceCount < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX11Texture::AddRef()
		{
			m_ReferenceCount++;
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX11Texture::Create(IDevice* pDevice, const ResourceData* const pInitialData, const TextureInfo& info)
		{
		}
	}
}

#endif