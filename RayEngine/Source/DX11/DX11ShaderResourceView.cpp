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

#include "../../Include/Debug/Debug.h"
#include "../../Include/DX11/DX11ShaderResourceView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "../../Include/DX11/DX11Device.h"
#include "../../Include/DX11/DX11Texture.h"
#include "../../Include/DX11/DX11Buffer.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11ShaderResourceView::DX11ShaderResourceView(IDevice* pDevice, const ShaderResourceViewDesc* pDesc)
			: m_Device(nullptr),
			m_View(nullptr),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX11Device*>(pDevice);

			Create( pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11ShaderResourceView::~DX11ShaderResourceView()
		{
			D3DRelease_S(m_View);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11ShaderResourceView::SetName(const std::string& name)
		{
			m_View->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11ShaderResourceView::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX11Device>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11ShaderResourceView::GetDesc(ShaderResourceViewDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11ShaderResourceView::GetReferenceCount() const
		{
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11ShaderResourceView::AddRef()
		{
			m_References++;
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11ShaderResourceView::Release()
		{
			m_References--;
			IObject::CounterType counter = m_References;

			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11ShaderResourceView::Create(const ShaderResourceViewDesc* pDesc)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
			desc.Format = ReToDXFormat(pDesc->Format);
			
			ID3D11Resource* pD3D11Resource = nullptr;
			if (pDesc->ViewDimension == VIEWDIMENSION_BUFFER)
			{
				if (pDesc->Flags & SHADER_RESOURCE_VIEW_FLAG_RAW_BUFFER)
				{
					desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
					desc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
					desc.BufferEx.FirstElement = pDesc->Buffer.StartElement;
					desc.BufferEx.NumElements = pDesc->Buffer.ElementCount;
				}
				else
				{
					desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
					desc.Buffer.FirstElement = pDesc->Buffer.StartElement;
					desc.Buffer.NumElements = pDesc->Buffer.ElementCount;
				}

				pD3D11Resource = reinterpret_cast<const DX11Buffer*>(pDesc->pResource)->GetD3D11Buffer();
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE1D)
			{
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
				desc.Texture1D.MipLevels = pDesc->Texture1D.MipLevels;
				desc.Texture1D.MostDetailedMip = pDesc->Texture1D.MostDetailedMip;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(pDesc->pResource)->GetD3D11Texture1D();
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE1D_ARRAY)
			{
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
				desc.Texture1DArray.MipLevels = pDesc->Texture1DArray.MipLevels;
				desc.Texture1DArray.MostDetailedMip = pDesc->Texture1DArray.MostDetailedMip;
				desc.Texture1DArray.ArraySize = pDesc->Texture1DArray.ArraySize;
				desc.Texture1DArray.FirstArraySlice = pDesc->Texture1DArray.FirstArraySlice;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(pDesc->pResource)->GetD3D11Texture1D();
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE2D)
			{
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipLevels = pDesc->Texture2D.MipLevels;
				desc.Texture2D.MostDetailedMip = pDesc->Texture2D.MostDetailedMip;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(pDesc->pResource)->GetD3D11Texture2D();
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE2DMS)
			{
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(pDesc->pResource)->GetD3D11Texture2D();
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE2D_ARRAY)
			{
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.MipLevels = pDesc->Texture2DArray.MipLevels;
				desc.Texture2DArray.MostDetailedMip = pDesc->Texture2DArray.MostDetailedMip;
				desc.Texture2DArray.ArraySize = pDesc->Texture2DArray.ArraySize;
				desc.Texture2DArray.FirstArraySlice = pDesc->Texture2DArray.FirstArraySlice;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(pDesc->pResource)->GetD3D11Texture2D();
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE2DMS_ARRAY)
			{
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
				desc.Texture2DMSArray.ArraySize = pDesc->Texture2DMSArray.ArraySize;
				desc.Texture2DMSArray.FirstArraySlice = pDesc->Texture2DMSArray.FirstArraySlice;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(pDesc->pResource)->GetD3D11Texture2D();
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE3D)
			{
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
				desc.Texture3D.MipLevels = pDesc->Texture3D.MipLevels;
				desc.Texture3D.MostDetailedMip = pDesc->Texture3D.MostDetailedMip;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(pDesc->pResource)->GetD3D11Texture3D();
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE_CUBEMAP)
			{
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
				desc.TextureCube.MipLevels= pDesc->TextureCube.MipLevels;
				desc.TextureCube.MostDetailedMip = pDesc->TextureCube.MostDetailedMip;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(pDesc->pResource)->GetD3D11Texture2D();
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE_CUBEMAP_ARRAY)
			{
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
				desc.TextureCubeArray.MipLevels = pDesc->TextureCube.MipLevels;
				desc.TextureCubeArray.MostDetailedMip = pDesc->TextureCubeArray.MostDetailedMip;
				desc.TextureCubeArray.First2DArrayFace = pDesc->TextureCubeArray.First2DArrayFace;
				desc.TextureCubeArray.NumCubes = pDesc->TextureCubeArray.CubeCount;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(pDesc->pResource)->GetD3D11Texture2D();
			}

		
			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			HRESULT hr = pD3D11Device->CreateShaderResourceView(pD3D11Resource, &desc, &m_View);
			if (FAILED(hr))
			{
				LOG_ERROR("D3D11: Could not create ShaderResourceView. " + DXErrorString(hr));
			}
			else
			{
				m_Desc = *pDesc;

				m_View->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(pDesc->Name.size()), pDesc->Name.c_str());
			}
		}
	}
}

#endif