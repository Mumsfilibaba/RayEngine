#include "..\..\Include\DX11\DX11RenderTargetView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"
#include "..\..\Include\DX11\DX11Texture.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11RenderTargetView::DX11RenderTargetView(IDevice* pDevice, const RenderTargetViewInfo& info)
			: m_Device(nullptr),
			m_View(nullptr)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());

			Create(pDevice, info);
		}



		/////////////////////////////////////////////////////////////
		DX11RenderTargetView::~DX11RenderTargetView()
		{
			D3DRelease_S(m_View);
			if (m_Device != nullptr)
			{
				m_Device->Release();
				m_Device = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		ID3D11RenderTargetView* DX11RenderTargetView::GetD3D11RenderTargetView() const
		{
			return m_View;
		}



		/////////////////////////////////////////////////////////////
		IDevice* DX11RenderTargetView::GetDevice() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		void DX11RenderTargetView::Create(IDevice* pDevice, const RenderTargetViewInfo& info)
		{
			using namespace System;

			D3D11_RENDER_TARGET_VIEW_DESC desc = {};
			desc.Format = ReToDXFormat(info.Format);
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = 0;

			ID3D11Texture2D* pD3D11Texture2D = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture2D();
			ID3D11Device* pD3D11Device = reinterpret_cast<DX11Device*>(pDevice)->GetD3D11Device();
			HRESULT hr = pD3D11Device->CreateRenderTargetView(pD3D11Texture2D, &desc, &m_View);
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create RenderTargetView. " + DXErrorString(hr));
				return;
			}
		}
	}
}

#endif