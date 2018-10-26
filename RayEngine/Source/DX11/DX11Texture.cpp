/*////////////////////////////////////////////////////////////

Copyright 2018 Alexander Dahlin

Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in
compliance with the License. You may obtain a copy of
the License at

http ://www.apache.org/licenses/LICENSE-2.0

THIS SOFTWARE IS PROVIDED "AS IS". MEANING NO WARRANTY
OR SUPPORT IS PROVIDED OF ANY KIND.

In event of any damages, direct or indirect that can
be traced back to the use of this software, shall no
contributor be held liable. This includes computer
failure and or malfunction of any kind.

////////////////////////////////////////////////////////////*/

#include "..\..\Include\System\Log\LogService.h"
#include "..\..\Include\DX11\DX11Texture.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11Texture::DX11Texture(IDevice* pDevice, const ResourceData* const pInitialData, const TextureDesc* pDesc)
			: m_Device(nullptr),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX11Device*>(pDevice);

			Create(pInitialData, pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11Texture::DX11Texture(IDevice* pDevice, ID3D11Texture2D* pResource)
			: m_Device(nullptr),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX11Device*>(pDevice);

			m_Texture2D = pResource;
			pResource->AddRef();

			D3D11_TEXTURE2D_DESC desc = {};
			pResource->GetDesc(&desc);

			m_Desc.Height = desc.Height;
			m_Desc.Width = desc.Width;
			m_Desc.MipLevels = desc.MipLevels;
			m_Desc.DepthOrArraySize = desc.ArraySize;
			m_Desc.Type = TEXTURE_TYPE_2D;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11Texture::~DX11Texture()
		{
			if (m_Desc.Type == TEXTURE_TYPE_1D) 
			{
				D3DRelease_S(m_Texture1D);
			}
			else if (m_Desc.Type == TEXTURE_TYPE_2D)
			{
				D3DRelease_S(m_Texture2D);
			}
			else if (m_Desc.Type == TEXTURE_TYPE_3D)
			{
				D3DRelease_S(m_Texture3D);
			}
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Texture::SetName(const std::string& name)
		{
			ID3D11DeviceChild* pDeviceChild = m_Texture1D;
			pDeviceChild->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());
		}

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Texture::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX11Device>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Texture::GetDesc(TextureDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11Texture::GetReferenceCount() const
		{
			return m_References;
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11Texture::AddRef()
		{
			m_References++;
			return m_References;
		}

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11Texture::Release()
		{
			m_References--;
			IObject::CounterType counter = m_References;

			if (counter < 1)
				delete this;

			return counter;
		}

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Texture::Create(const ResourceData* const pInitialData, const TextureDesc* pDesc)
		{
			DXGI_FORMAT format = ReToDXFormat(pDesc->Format);

			uint32 bindFlags = 0;
			if (pDesc->Flags & TEXTURE_FLAGS_DEPTH_STENCIL)
				bindFlags = D3D11_BIND_DEPTH_STENCIL;
			else if (pDesc->Flags & TEXTURE_FLAGS_RENDERTARGET)
				bindFlags = D3D11_BIND_RENDER_TARGET;
			else if (pDesc->Flags == TEXTURE_FLAGS_SHADER_RESOURCE)
				bindFlags = D3D11_BIND_SHADER_RESOURCE;
			else if (pDesc->Flags == TEXTURE_FLAGS_UNORDERED_ACCESS)
				bindFlags = D3D11_BIND_UNORDERED_ACCESS;


			uint32 cpuAccessFlags = 0;
			if (pDesc->CpuAccess & CPU_ACCESS_FLAG_WRITE)
				cpuAccessFlags |= D3D11_CPU_ACCESS_WRITE;
			if (pDesc->CpuAccess & CPU_ACCESS_FLAG_READ)
				cpuAccessFlags |= D3D11_CPU_ACCESS_READ;


			D3D11_USAGE usage;
			if (pDesc->Usage == RESOURCE_USAGE_DEFAULT)
				usage = D3D11_USAGE_DEFAULT;
			else if (pDesc->Usage == RESOURCE_USAGE_DYNAMIC)
				usage = D3D11_USAGE_DYNAMIC;
			else if (pDesc->Usage == RESOURCE_USAGE_STATIC)
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
			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			ID3D11DeviceChild* pD3D11DeviceChild = nullptr;

			if (pDesc->Type == TEXTURE_TYPE_1D)
			{
				D3D11_TEXTURE1D_DESC desc = {};
				desc.Format = format;
				desc.BindFlags = bindFlags;
				desc.CPUAccessFlags = cpuAccessFlags;
				desc.Usage = usage;
				desc.MiscFlags = miscFlags;
				desc.Width = pDesc->Width;
				desc.MipLevels = pDesc->MipLevels;
				desc.ArraySize = pDesc->DepthOrArraySize;

				hr = pD3D11Device->CreateTexture1D(&desc, pInitData, &m_Texture1D);
				pD3D11DeviceChild = m_Texture1D;
			}
			else if (pDesc->Type == TEXTURE_TYPE_2D)
			{
				D3D11_TEXTURE2D_DESC desc = {};
				desc.Format = format;
				desc.BindFlags = bindFlags;
				desc.CPUAccessFlags = cpuAccessFlags;
				desc.Usage = usage;
				desc.MiscFlags = miscFlags;
				desc.Width = pDesc->Width;
				desc.Height = pDesc->Height;
				desc.SampleDesc.Count = pDesc->SampleCount;

				desc.SampleDesc.Quality = 0;
				GetHighestSupportingSamples(pD3D11Device, &desc.SampleDesc.Count, &desc.SampleDesc.Quality, pDesc->SampleCount, format);
				
				desc.MipLevels = pDesc->MipLevels;
				desc.ArraySize = pDesc->DepthOrArraySize;

				hr = pD3D11Device->CreateTexture2D(&desc, pInitData, &m_Texture2D);
				pD3D11DeviceChild = m_Texture2D;
			}
			else if (pDesc->Type == TEXTURE_TYPE_3D)
			{
				D3D11_TEXTURE3D_DESC desc = {};
				desc.Format = format;
				desc.BindFlags = bindFlags;
				desc.CPUAccessFlags = cpuAccessFlags;
				desc.Usage = usage;
				desc.MiscFlags = miscFlags;
				desc.Height = pDesc->Height;
				desc.Width = pDesc->Width;
				desc.MipLevels = pDesc->MipLevels;
				desc.Depth = pDesc->DepthOrArraySize;

				hr = pD3D11Device->CreateTexture3D(&desc, pInitData, &m_Texture3D);
				pD3D11DeviceChild = m_Texture3D;
			}


			if (FAILED(hr))
			{
				LogService::GraphicsLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create texture. " + DXErrorString(hr));
			}
			else
			{
				m_Desc = *pDesc;

				pD3D11DeviceChild->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(pDesc->Name.size()), pDesc->Name.c_str());
			}
		}
	}
}

#endif