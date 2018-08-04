#include "..\..\Include\DX11\DX11Texture.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11Texture::DX11Texture(IDevice* pDevice, const ResourceData* const pInitialData, const TextureInfo& info)
			: m_Device(nullptr),
			m_Type(TEXTURE_TYPE_UNKNOWN)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());

			Create(pDevice, pInitialData, info);
		}



		/////////////////////////////////////////////////////////////
		DX11Texture::DX11Texture(IDevice* pDevice, ID3D11Texture2D* pResource)
			: m_Device(nullptr),
			m_Type(TEXTURE_TYPE_2D)
		{
			AddRef();

			pResource->AddRef();
			m_Texture2D = pResource;
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

			ReRelease_S(m_Device);
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
		void DX11Texture::Create(IDevice* pDevice, const ResourceData* const pInitialData, const TextureInfo& info)
		{
			using namespace System;
			DXGI_FORMAT format = ReToDXFormat(info.Format);

			uint32 bindFlags = 0;
			if (info.Flags == TEXTURE_FLAGS_DEPTHBUFFER)
				bindFlags = D3D11_BIND_DEPTH_STENCIL;
			else if (info.Flags == TEXTURE_FLAGS_DEPTHBUFFER)
				bindFlags = D3D11_BIND_DEPTH_STENCIL;
			else if (info.Flags == TEXTURE_FLAGS_DEPTHBUFFER)
				bindFlags = D3D11_BIND_DEPTH_STENCIL;

			uint32 cpuAccessFlags = 0;
			if (info.CpuAccess & CPU_ACCESS_FLAG_WRITE)
				cpuAccessFlags |= D3D11_CPU_ACCESS_WRITE;
			if (info.CpuAccess & CPU_ACCESS_FLAG_READ)
				cpuAccessFlags |= D3D11_CPU_ACCESS_READ;

			D3D11_USAGE usage;
			if (info.Usage == RESOURCE_USAGE_DEFAULT)
				usage = D3D11_USAGE_DEFAULT;
			else if (info.Usage == RESOURCE_USAGE_DYNAMIC)
				usage = D3D11_USAGE_DYNAMIC;
			else if (info.Usage == RESOURCE_USAGE_STATIC)
				usage = D3D11_USAGE_IMMUTABLE;

			uint32 miscFlags = 0;

			
			D3D11_SUBRESOURCE_DATA data = {};
			D3D11_SUBRESOURCE_DATA* pInitData = nullptr;
			if (pInitialData != nullptr)
			{
				data.pSysMem = pInitialData->pData;
				data.SysMemPitch = pInitialData->WidthOrCount * pInitialData->ByteStride;
				data.SysMemSlicePitch = data.SysMemPitch * pInitialData->Height;

				pInitData = &data;
			}


			HRESULT hr = 0;
			ID3D11Device* pD3D11Device = reinterpret_cast<DX11Device*>(pDevice)->GetD3D11Device();

			if (info.Type == TEXTURE_TYPE_1D)
			{
				D3D11_TEXTURE1D_DESC desc = {};
				desc.Format = format;
				desc.BindFlags = bindFlags;
				desc.CPUAccessFlags = cpuAccessFlags;
				desc.Usage = usage;
				desc.MiscFlags = miscFlags;
				desc.Width = info.Width;
				desc.MipLevels = info.MipLevels;
				desc.ArraySize = info.DepthOrArraySize;

				hr = pD3D11Device->CreateTexture1D(&desc, pInitData, &m_Texture1D);
			}
			else if (info.Type == TEXTURE_TYPE_2D)
			{
				D3D11_TEXTURE2D_DESC desc = {};
				desc.Format = format;
				desc.BindFlags = bindFlags;
				desc.CPUAccessFlags = cpuAccessFlags;
				desc.Usage = usage;
				desc.MiscFlags = miscFlags;
				desc.Width = info.Width;
				desc.Height = info.Height;
				desc.SampleDesc.Count = info.SampleCount;
				desc.SampleDesc.Quality = 0;
				desc.MipLevels = info.MipLevels;
				desc.ArraySize = info.DepthOrArraySize;

				hr = pD3D11Device->CreateTexture2D(&desc, pInitData, &m_Texture2D);
			}
			else if (info.Type == TEXTURE_TYPE_3D)
			{
				D3D11_TEXTURE3D_DESC desc = {};
				desc.Format = format;
				desc.BindFlags = bindFlags;
				desc.CPUAccessFlags = cpuAccessFlags;
				desc.Usage = usage;
				desc.MiscFlags = miscFlags;
				desc.Height = info.Height;
				desc.Width = info.Width;
				desc.MipLevels = info.MipLevels;
				desc.Depth = info.DepthOrArraySize;

				hr = pD3D11Device->CreateTexture3D(&desc, pInitData, &m_Texture3D);
			}


			if (FAILED(hr))
			{
				pDevice->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create texture. " + DXErrorString(hr));
				return;
			}
		}
	}
}

#endif