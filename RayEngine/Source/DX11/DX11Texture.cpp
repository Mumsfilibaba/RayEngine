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

#include <RayEngine.h>

#if defined(RE_PLATFORM_WINDOWS)
#include <DX11/DX11Texture.h>
#include <DX11/DX11Device.h>

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

			m_Desc.Type = TEXTURE_TYPE_2D;
			m_Desc.Format = DXToReFormat(desc.Format);
			m_Desc.Usage = DX11ToReUsage(desc.Usage);

			m_Desc.Flags = 0;
			if (desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
				m_Desc.Flags |= TEXTURE_FLAGS_DEPTH_STENCIL;
			if (desc.BindFlags & D3D11_BIND_RENDER_TARGET)
				m_Desc.Flags |= TEXTURE_FLAGS_RENDERTARGET;
			if (desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
				m_Desc.Flags |= TEXTURE_FLAGS_SHADER_RESOURCE;
			if (desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
				m_Desc.Flags |= TEXTURE_FLAGS_UNORDERED_ACCESS;

			if (desc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE)
				m_Desc.Flags |= TEXTURE_FLAGS_CUBEMAP;

			if (desc.CPUAccessFlags & D3D11_CPU_ACCESS_WRITE)
				m_Desc.CpuAccess |= CPU_ACCESS_FLAG_WRITE;
			if (desc.CPUAccessFlags & D3D11_CPU_ACCESS_READ)
				m_Desc.CpuAccess |= CPU_ACCESS_FLAG_READ;

			m_Desc.Height = desc.Height;
			m_Desc.Width = desc.Width;
			m_Desc.DepthOrArraySize = desc.ArraySize;
			m_Desc.SampleCount = desc.SampleDesc.Count;
			m_Desc.MipLevels = desc.MipLevels;
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
		void DX11Texture::GetDesc(TextureDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX11Texture::AddRef()
		{
			return ++m_References;
		}

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX11Texture::Release()
		{
			CounterType counter = --m_References;
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

			D3D11_USAGE usage = ReToDX11Usage(pDesc->Usage);
			
			uint32 miscFlags = 0;
			if (pDesc->Flags & TEXTURE_FLAGS_CUBEMAP)
				miscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;

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
				LOG_ERROR( "D3D11: Could not create texture. " + DXErrorString(hr));
			}
			else
			{
				m_Desc = *pDesc;
			}
		}
	}
}

#endif