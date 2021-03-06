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
#include <DX11/DX11Device.h>
#include <DX11/DX11Buffer.h>
#include <DX11/DX11Texture.h>
#include <DX11/DX11UnorderedAccessView.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11UnorderedAccessView::DX11UnorderedAccessView(IDevice* pDevice, const UnorderedAccessViewDesc* pDesc)
			: m_Device(nullptr),
			m_View(nullptr),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX11Device*>(pDevice);

			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11UnorderedAccessView::~DX11UnorderedAccessView()
		{
			D3DRelease_S(m_View);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11UnorderedAccessView::GetDesc(UnorderedAccessViewDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX11UnorderedAccessView::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX11UnorderedAccessView::Release()
		{
			CounterType counter = --m_References;
			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11UnorderedAccessView::Create(const UnorderedAccessViewDesc* pDesc)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC desc = {};
			desc.Format = ReToDXFormat(pDesc->Format);

			ID3D11Resource* pD3D11Resource = nullptr;
			if (pDesc->ViewDimension == VIEWDIMENSION_BUFFER)
			{
				desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
				desc.Buffer.Flags = 0;

				if (pDesc->Flags & UNORDERED_ACCESS_VIEW_FLAG_BUFFER_RAW)
					desc.Buffer.Flags |= D3D11_BUFFER_UAV_FLAG_RAW;
				if (pDesc->Flags & UNORDERED_ACCESS_VIEW_FLAG_BUFFER_COUNTER)
					desc.Buffer.Flags |= D3D11_BUFFER_UAV_FLAG_COUNTER;

				desc.Buffer.FirstElement = pDesc->Buffer.StartElement;
				desc.Buffer.NumElements = pDesc->Buffer.ElementCount;

				pD3D11Resource = reinterpret_cast<const DX11Buffer*>(pDesc->pResource)->GetD3D11Buffer();
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE1D)
			{
				desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1D;
				desc.Texture1D.MipSlice = pDesc->Texture1D.MipSlice;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(pDesc->pResource)->GetD3D11Texture1D();
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE1D_ARRAY)
			{
				desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1DARRAY;
				desc.Texture1DArray.MipSlice = pDesc->Texture1DArray.MipSlice;
				desc.Texture1DArray.ArraySize = pDesc->Texture1DArray.ArraySize;
				desc.Texture1DArray.FirstArraySlice = pDesc->Texture1DArray.FirstArraySlice;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(pDesc->pResource)->GetD3D11Texture1D();
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE2D)
			{
				desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipSlice = pDesc->Texture2D.MipSlice;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(pDesc->pResource)->GetD3D11Texture2D();
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE2D_ARRAY)
			{
				desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.MipSlice = pDesc->Texture2DArray.MipSlice;
				desc.Texture2DArray.ArraySize = pDesc->Texture2DArray.ArraySize;
				desc.Texture2DArray.FirstArraySlice = pDesc->Texture2DArray.FirstArraySlice;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(pDesc->pResource)->GetD3D11Texture2D();
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE3D)
			{
				desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
				desc.Texture3D.MipSlice = pDesc->Texture3D.MipSlice;
				desc.Texture3D.FirstWSlice = pDesc->Texture3D.FirstDepthSlice;
				desc.Texture3D.WSize = pDesc->Texture3D.DepthSliceCount;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(pDesc->pResource)->GetD3D11Texture3D();
			}

			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();

			HRESULT hr = pD3D11Device->CreateUnorderedAccessView(pD3D11Resource, &desc, &m_View);
			if (FAILED(hr))
			{
				LOG_ERROR("D3D11: Could not create UnorderedAccessView. " + DXErrorString(hr));
			}
			else
			{
				m_Desc = *pDesc;
			}
		}
	}
}

#endif