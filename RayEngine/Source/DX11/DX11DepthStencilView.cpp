#include "..\..\Include\DX11\DX11DepthStencilView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"
#include "..\..\Include\DX11\DX11Texture.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11DepthStencilView::DX11DepthStencilView(IDevice* pDevice, const DepthStencilViewInfo& info)
			: m_Device(nullptr),
			m_View(nullptr)
		{
			AddRef();
			m_Device = QueryDX11Device(pDevice);

			Create(info);
		}



		/////////////////////////////////////////////////////////////
		DX11DepthStencilView::~DX11DepthStencilView()
		{
			D3DRelease_S(m_View);

			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		ID3D11DepthStencilView* DX11DepthStencilView::GetD3D11DepthStencilView() const
		{
			return m_View;
		}



		/////////////////////////////////////////////////////////////
		void DX11DepthStencilView::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryDX11Device(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX11DepthStencilView::Create(const DepthStencilViewInfo& info)
		{
			using namespace System;

			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			ID3D11Resource* pD3D11Resource = nullptr; 

			D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};
			desc.Format = ReToDXFormat(info.Format);
			desc.Flags = 0;
			if (info.Flags & DEPTH_STENCIL_VIEW_FLAGS_READ_ONLY_STENCIL)
				desc.Flags |= D3D11_DSV_READ_ONLY_STENCIL;
			else if (info.Flags & DEPTH_STENCIL_VIEW_FLAGS_READ_ONLY_DEPTH)
				desc.Flags |= D3D11_DSV_READ_ONLY_DEPTH;


			if (info.ViewDimension == VIEWDIMENSION_TEXTURE1D)
			{
				desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1D;
				desc.Texture1D.MipSlice = info.Texture1D.MipSlice;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture1D();
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE1D_ARRAY)
			{
				desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1DARRAY;
				desc.Texture1DArray.MipSlice = info.Texture1DArray.MipSlice;
				desc.Texture1DArray.ArraySize = info.Texture1DArray.ArraySize;
				desc.Texture1DArray.FirstArraySlice = info.Texture1DArray.FirstArraySlice;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture1D();
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE2D)
			{
				desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipSlice = info.Texture2D.MipSlice;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture2D();
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE2D_ARRAY)
			{
				desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.MipSlice = info.Texture2DArray.MipSlice;
				desc.Texture2DArray.ArraySize = info.Texture2DArray.ArraySize;
				desc.Texture2DArray.FirstArraySlice = info.Texture2DArray.FirstArraySlice;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture2D();
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE2DMS)
			{
				desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture2D();
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE2DMS_ARRAY)
			{
				desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
				desc.Texture2DMSArray.ArraySize = info.Texture2DMSArray.ArraySize;
				desc.Texture2DMSArray.FirstArraySlice = info.Texture2DMSArray.FirstArraySlice;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture2D();
			}


			HRESULT hr = pD3D11Device->CreateDepthStencilView(pD3D11Resource, &desc, &m_View);
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create DepthStencilView" + DXErrorString(hr));
			}
			else
			{
				m_View->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(info.Name.size()), info.Name.c_str());
			}
		}
	}
}

#endif