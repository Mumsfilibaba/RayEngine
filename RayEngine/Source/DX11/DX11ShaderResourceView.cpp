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

#include "..\..\Include\DX11\DX11ShaderResourceView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"
#include "..\..\Include\DX11\DX11Texture.h"
#include "..\..\Include\DX11\DX11Buffer.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11ShaderResourceView::DX11ShaderResourceView(IDevice* pDevice, const ShaderResourceViewInfo& info)
			: m_Device(nullptr),
			m_View(nullptr),
			m_References(0)
		{
			AddRef();
			m_Device = pDevice->QueryReference<DX11Device>();

			Create( info);
		}



		/////////////////////////////////////////////////////////////
		DX11ShaderResourceView::~DX11ShaderResourceView()
		{
			D3DRelease_S(m_View);
		}



		/////////////////////////////////////////////////////////////
		ID3D11ShaderResourceView* DX11ShaderResourceView::GetD3D11ShaderResourceView() const
		{
			return m_View;
		}



		/////////////////////////////////////////////////////////////
		void DX11ShaderResourceView::SetName(const std::string& name)
		{
			m_View->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());
		}



		/////////////////////////////////////////////////////////////
		void DX11ShaderResourceView::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX11Device>();
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType DX11ShaderResourceView::GetReferenceCount() const
		{
			return m_References;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType DX11ShaderResourceView::AddRef()
		{
			m_References++;
			return m_References;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType DX11ShaderResourceView::Release()
		{
			IObject::CounterType counter = m_References--;
			if (m_References < 1)
				delete this;

			return counter;
		}



		/////////////////////////////////////////////////////////////
		void DX11ShaderResourceView::Create(const ShaderResourceViewInfo& info)
		{
			using namespace System;

			D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
			desc.Format = ReToDXFormat(info.Format);
			
			ID3D11Resource* pD3D11Resource = nullptr;
			if (info.ViewDimension == VIEWDIMENSION_BUFFER)
			{
				if (info.Flags & SHADER_RESOURCE_VIEW_FLAG_RAW_BUFFER)
				{
					desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
					desc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
					desc.BufferEx.FirstElement = info.Buffer.StartElement;
					desc.BufferEx.NumElements = info.Buffer.ElementCount;
				}
				else
				{
					desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
					desc.Buffer.FirstElement = info.Buffer.StartElement;
					desc.Buffer.NumElements = info.Buffer.ElementCount;
				}

				pD3D11Resource = reinterpret_cast<const DX11Buffer*>(info.pResource)->GetD3D11Buffer();
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE1D)
			{
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
				desc.Texture1D.MipLevels = info.Texture1D.MipLevels;
				desc.Texture1D.MostDetailedMip = info.Texture1D.MostDetailedMip;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture1D();
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE1D_ARRAY)
			{
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
				desc.Texture1DArray.MipLevels = info.Texture1DArray.MipLevels;
				desc.Texture1DArray.MostDetailedMip = info.Texture1DArray.MostDetailedMip;
				desc.Texture1DArray.ArraySize = info.Texture1DArray.ArraySize;
				desc.Texture1DArray.FirstArraySlice = info.Texture1DArray.FirstArraySlice;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture1D();
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE2D)
			{
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipLevels = info.Texture2D.MipLevels;
				desc.Texture2D.MostDetailedMip = info.Texture2D.MostDetailedMip;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture2D();
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE2DMS)
			{
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture2D();
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE2D_ARRAY)
			{
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.MipLevels = info.Texture2DArray.MipLevels;
				desc.Texture2DArray.MostDetailedMip = info.Texture2DArray.MostDetailedMip;
				desc.Texture2DArray.ArraySize = info.Texture2DArray.ArraySize;
				desc.Texture2DArray.FirstArraySlice = info.Texture2DArray.FirstArraySlice;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture2D();
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE2DMS_ARRAY)
			{
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
				desc.Texture2DMSArray.ArraySize = info.Texture2DMSArray.ArraySize;
				desc.Texture2DMSArray.FirstArraySlice = info.Texture2DMSArray.FirstArraySlice;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture2D();
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE3D)
			{
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
				desc.Texture3D.MipLevels = info.Texture3D.MipLevels;
				desc.Texture3D.MostDetailedMip = info.Texture3D.MostDetailedMip;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture3D();
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE_CUBEMAP)
			{
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
				desc.TextureCube.MipLevels= info.TextureCube.MipLevels;
				desc.TextureCube.MostDetailedMip = info.TextureCube.MostDetailedMip;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture2D();
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE_CUBEMAP_ARRAY)
			{
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
				desc.TextureCubeArray.MipLevels = info.TextureCube.MipLevels;
				desc.TextureCubeArray.MostDetailedMip = info.TextureCubeArray.MostDetailedMip;
				desc.TextureCubeArray.First2DArrayFace = info.TextureCubeArray.First2DArrayFace;
				desc.TextureCubeArray.NumCubes = info.TextureCubeArray.CubeCount;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture2D();
			}

		
			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			
			HRESULT hr = pD3D11Device->CreateShaderResourceView(pD3D11Resource, &desc, &m_View);
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create ShaderResourceView. " + DXErrorString(hr));
			}
			else
			{
				m_View->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(info.Name.size()), info.Name.c_str());
			}
		}
	}
}

#endif