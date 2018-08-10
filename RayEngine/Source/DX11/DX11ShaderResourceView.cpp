#include "..\..\Include\DX11\DX11ShaderResourceView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"
#include "..\..\Include\DX11\DX11Texture.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11ShaderResourceView::DX11ShaderResourceView(IDevice* pDevice, const ShaderResourceViewInfo& info)
			: m_Device(nullptr),
			m_View(nullptr)
		{
			AddRef();
			m_Device = QueryDX11Device(pDevice);

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
		void DX11ShaderResourceView::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryDX11Device(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX11ShaderResourceView::Create(const ShaderResourceViewInfo& info)
		{
			using namespace System;

			//TODO: Get miplevel info and not only Texture2D
			D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MostDetailedMip = 0;
			desc.Texture2D.MipLevels = 1;

			
			ID3D11Texture2D* pD3D11Texture2D = reinterpret_cast<const DX11Texture*>(info.pResource)->GetD3D11Texture2D();
			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			
			HRESULT hr = pD3D11Device->CreateShaderResourceView(pD3D11Texture2D, &desc, &m_View);
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