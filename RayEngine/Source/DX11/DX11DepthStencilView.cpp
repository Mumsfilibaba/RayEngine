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
			m_View(nullptr),
			m_ReferenceCount(0)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());

			Create(pDevice, info);
		}



		/////////////////////////////////////////////////////////////
		DX11DepthStencilView::DX11DepthStencilView(DX11DepthStencilView&& other)
			: m_Device(other.m_Device),
			m_View(other.m_View),
			m_ReferenceCount(other.m_ReferenceCount)
		{
			other.m_Device = nullptr;
			other.m_View = nullptr;
			other.m_ReferenceCount = 0;
		}



		/////////////////////////////////////////////////////////////
		DX11DepthStencilView::~DX11DepthStencilView()
		{
			D3DRelease_S(m_View);
			if (m_Device != nullptr)
			{
				m_Device->Release();
				m_Device = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		ID3D11DepthStencilView* DX11DepthStencilView::GetD3D11DepthStencilView() const
		{
			return m_View;
		}



		/////////////////////////////////////////////////////////////
		DX11DepthStencilView& DX11DepthStencilView::operator=(DX11DepthStencilView && other)
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
		IDevice* DX11DepthStencilView::GetDevice() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		IReferenceCounter* DX11DepthStencilView::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX11DepthStencilView::GetReferenceCount() const
		{
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX11DepthStencilView::Release() const
		{
			m_ReferenceCount--;
			if (m_ReferenceCount < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX11DepthStencilView::AddRef()
		{
			m_ReferenceCount++;
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX11DepthStencilView::Create(IDevice* pDevice, const DepthStencilViewInfo& info)
		{
			using namespace System;

			ID3D11Device* pD3D11Device = reinterpret_cast<DX11Device*>(pDevice)->GetD3D11Device();
			ID3D11Texture2D* pD3D11Texture = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture2D();

			D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};
			desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = 0;
			desc.Format = ReToDXFormat(info.Format);

			HRESULT hr = pD3D11Device->CreateDepthStencilView(pD3D11Texture, &desc, &m_View);
			if (FAILED(hr))
			{
				pDevice->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create DepthStencilView" + DXErrorString(hr));
				return;
			}
		}
	}
}

#endif