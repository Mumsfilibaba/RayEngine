#include "..\..\Include\DX11\DX11UnorderedAccessView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"
#include "..\..\Include\DX11\DX11Texture.h"

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

			//TODO: Test
			D3D11_UNORDERED_ACCESS_VIEW_DESC desc = {};
			desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = 0;

			ID3D11Texture2D* pD3D11Texture2D = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture2D();
			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();

			HRESULT hr = pD3D11Device->CreateUnorderedAccessView(pD3D11Texture2D, &desc, &m_View);
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