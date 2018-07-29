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
			m_View(nullptr),
			m_ReferenceCount(0)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());

			Create(pDevice, info);
		}



		/////////////////////////////////////////////////////////////
		DX11RenderTargetView::DX11RenderTargetView(DX11RenderTargetView&& other)
			: m_Device(other.m_Device),
			m_View(other.m_View),
			m_ReferenceCount(other.m_ReferenceCount)
		{
			other.m_Device = nullptr;
			other.m_View = nullptr;
			other.m_ReferenceCount = 0;
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
		DX11RenderTargetView& DX11RenderTargetView::operator=(DX11RenderTargetView&& other)
		{
			if (this != &other)
			{
				D3DRelease_S(m_View);
				if (m_Device != nullptr)
				{
					m_Device->Release();
					m_Device = nullptr;
				}


				m_Device = other.m_Device;
				m_View = other.m_View;
				m_ReferenceCount = other.m_ReferenceCount;


				other.m_Device = nullptr;
				other.m_View = nullptr;
				other.m_ReferenceCount = 0;
			}

			return *this;
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
		IReferenceCounter* DX11RenderTargetView::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX11RenderTargetView::GetReferenceCount() const
		{
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX11RenderTargetView::Release() const
		{
			m_ReferenceCount--;
			if (m_ReferenceCount < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX11RenderTargetView::AddRef()
		{
			m_ReferenceCount++;
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX11RenderTargetView::Create(IDevice* pDevice, const RenderTargetViewInfo& info)
		{
			using namespace System;

			D3D11_RENDER_TARGET_VIEW_DESC desc = {};
			desc.Format = ReToDXFormat(info.Format);
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = 0;

			ID3D11Texture2D* pD3D11Texture2D = reinterpret_cast<const DX11Texture*>(info.Resource)->GetD3D11Texture2D();
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