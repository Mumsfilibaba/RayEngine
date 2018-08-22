#include "..\..\Include\DX11\DX11UnorderedAccessView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"
#include "..\..\Include\DX11\DX11Texture.h"
#include "..\..\Include\DX11\DX11Buffer.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11UnorderedAccessView::DX11UnorderedAccessView(IDevice* pDevice, const UnorderedAccessViewInfo& info)
			: m_Device(nullptr),
			m_View(nullptr)
		{
			AddRef();
			m_Device = QueryDX11Device(pDevice);

			Create(info);
		}



		/////////////////////////////////////////////////////////////
		DX11UnorderedAccessView::~DX11UnorderedAccessView()
		{
			D3DRelease_S(m_View);

			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		ID3D11UnorderedAccessView* DX11UnorderedAccessView::GetD3D11UnorderedAccessView() const
		{
			return m_View;
		}



		/////////////////////////////////////////////////////////////
		void DX11UnorderedAccessView::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryDX11Device(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX11UnorderedAccessView::Create(const UnorderedAccessViewInfo& info)
		{
			using namespace System;

			D3D11_UNORDERED_ACCESS_VIEW_DESC desc = {};
			desc.Format = ReToDXFormat(info.Format);

			ID3D11Resource* pD3D11Resource = nullptr;
			if (info.ViewDimension == VIEWDIMENSION_BUFFER)
			{
				desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
				desc.Buffer.Flags = 0;

				if (info.Flags & UNORDERED_ACCESS_VIEW_FLAG_BUFFER_RAW)
					desc.Buffer.Flags |= D3D11_BUFFER_UAV_FLAG_RAW;
				if (info.Flags & UNORDERED_ACCESS_VIEW_FLAG_BUFFER_COUNTER)
					desc.Buffer.Flags |= D3D11_BUFFER_UAV_FLAG_COUNTER;

				desc.Buffer.FirstElement = info.Buffer.StartElement;
				desc.Buffer.NumElements = info.Buffer.ElementCount;

				pD3D11Resource = reinterpret_cast<const DX11Buffer*>(info.pResource)->GetD3D11Buffer();
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE1D)
			{
				desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1D;
				desc.Texture1D.MipSlice = info.Texture1D.MipSlice;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture1D();
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE1D_ARRAY)
			{
				desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1DARRAY;
				desc.Texture1DArray.MipSlice = info.Texture1DArray.MipSlice;
				desc.Texture1DArray.ArraySize = info.Texture1DArray.ArraySize;
				desc.Texture1DArray.FirstArraySlice = info.Texture1DArray.FirstArraySlice;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture1D();
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE2D)
			{
				desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipSlice = info.Texture2D.MipSlice;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture2D();
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE2D_ARRAY)
			{
				desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.MipSlice = info.Texture2DArray.MipSlice;
				desc.Texture2DArray.ArraySize = info.Texture2DArray.ArraySize;
				desc.Texture2DArray.FirstArraySlice = info.Texture2DArray.FirstArraySlice;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture2D();
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE3D)
			{
				desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
				desc.Texture3D.MipSlice = info.Texture3D.MipSlice;
				desc.Texture3D.FirstWSlice = info.Texture3D.FirstDepthSlice;
				desc.Texture3D.WSize = info.Texture3D.DepthSliceCount;

				pD3D11Resource = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture3D();
			}

			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();

			HRESULT hr = pD3D11Device->CreateUnorderedAccessView(pD3D11Resource, &desc, &m_View);
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create UnorderedAccessView. " + DXErrorString(hr));
			}
			else
			{
				m_View->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(info.Name.size()), info.Name.c_str());
			}
		}
	}
}

#endif