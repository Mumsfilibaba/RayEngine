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

#include "..\..\Include\DX11\DX11Texture.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11Texture::DX11Texture(IDevice* pDevice, const ResourceData* const pInitialData, const TextureDesc& info)
			: m_Device(nullptr),
			m_Type(TEXTURE_TYPE_UNKNOWN),
			mReferences(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX11Device*>(pDevice);

			Create(pInitialData, info);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11Texture::DX11Texture(IDevice* pDevice, ID3D11Texture2D* pResource)
			: m_Device(nullptr),
			m_Type(TEXTURE_TYPE_2D),
			mReferences(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX11Device*>(pDevice);

			pResource->AddRef();
			m_Texture2D = pResource;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ID3D11Texture1D* DX11Texture::GetD3D11Texture1D() const
		{
			return m_Texture1D;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////s
		ID3D11Texture2D* DX11Texture::GetD3D11Texture2D() const
		{
			return m_Texture2D;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ID3D11Texture3D* DX11Texture::GetD3D11Texture3D() const
		{
			return m_Texture3D;
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
		IObject::CounterType DX11Texture::GetReferenceCount() const
		{
			return mReferences;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11Texture::AddRef()
		{
			mReferences++;
			return mReferences;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11Texture::Release()
		{
			mReferences--;
			IObject::CounterType counter = mReferences;

			if (counter < 1)
				delete this;

			return counter;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Texture::Create(const ResourceData* const pInitialData, const TextureDesc& info)
		{
			using namespace System;
			DXGI_FORMAT format = ReToDXFormat(info.Format);

			uint32 bindFlags = 0;
			if (info.Flags & TEXTURE_FLAGS_DEPTH_STENCIL)
				bindFlags = D3D11_BIND_DEPTH_STENCIL;
			else if (info.Flags & TEXTURE_FLAGS_RENDERTARGET)
				bindFlags = D3D11_BIND_RENDER_TARGET;
			else if (info.Flags == TEXTURE_FLAGS_SHADER_RESOURCE)
				bindFlags = D3D11_BIND_SHADER_RESOURCE;
			else if (info.Flags == TEXTURE_FLAGS_UNORDERED_ACCESS)
				bindFlags = D3D11_BIND_UNORDERED_ACCESS;


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
			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			ID3D11DeviceChild* pD3D11DeviceChild = nullptr;

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
				pD3D11DeviceChild = m_Texture1D;
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
				GetHighestSupportingSamples(pD3D11Device, &desc.SampleDesc.Count, &desc.SampleDesc.Quality, info.SampleCount, format);
				
				desc.MipLevels = info.MipLevels;
				desc.ArraySize = info.DepthOrArraySize;

				hr = pD3D11Device->CreateTexture2D(&desc, pInitData, &m_Texture2D);
				pD3D11DeviceChild = m_Texture2D;
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
				pD3D11DeviceChild = m_Texture3D;
			}


			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create texture. " + DXErrorString(hr));
			}
			else
			{
				pD3D11DeviceChild->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(info.Name.size()), info.Name.c_str());
			}
		}
	}
}

#endif