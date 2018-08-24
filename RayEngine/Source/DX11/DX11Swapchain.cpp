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

#include "..\..\Include\DX11\DX11Swapchain.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\Win32\Win32WindowImpl.h"
#include "..\..\Include\DX11\DX11Factory.h"
#include "..\..\Include\DX11\DX11Device.h"
#include "..\..\Include\DX11\DX11Texture.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11Swapchain::DX11Swapchain(IFactory* pFactory, const SwapchainInfo& info)
			: m_Device(nullptr),
			m_Factory(nullptr),
			m_Swapchain(nullptr),
			m_Texture(nullptr),
			m_BufferCount(0),
			m_CurrentBuffer(0)
		{
			AddRef();
			m_Factory = reinterpret_cast<DX11Factory*>(pFactory->QueryReference());
			m_Device = reinterpret_cast<DX11Device*>(info.pDevice->QueryReference());

			Create(info);
		}



		/////////////////////////////////////////////////////////////
		DX11Swapchain::~DX11Swapchain()
		{
			D3DRelease_S(m_Swapchain);
			
			ReRelease_S(m_Device);
			ReRelease_S(m_Factory);
			ReRelease_S(m_Texture);
		}



		/////////////////////////////////////////////////////////////
		int32 DX11Swapchain::GetCurrentBuffer() const
		{
			return m_CurrentBuffer;
		}



		/////////////////////////////////////////////////////////////
		ITexture* DX11Swapchain::GetBuffer(int32 index)
		{
			return m_Texture;
		}



		/////////////////////////////////////////////////////////////
		const ITexture* DX11Swapchain::GetBuffer(int32 index) const
		{
			return m_Texture;
		}



		/////////////////////////////////////////////////////////////
		void DX11Swapchain::SetName(const std::string& name)
		{
			m_Swapchain->SetPrivateData(WKPDID_D3DDebugObjectName, name.size(), name.c_str());
		}



		/////////////////////////////////////////////////////////////
		void DX11Swapchain::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryDX11Device(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX11Swapchain::QueryFactory(IFactory ** ppFactory) const
		{
			(*ppFactory) = reinterpret_cast<DX11Factory*>(m_Factory->QueryReference());
		}



		/////////////////////////////////////////////////////////////
		void DX11Swapchain::Present() const
		{
			m_Swapchain->Present(0, 0);
			
			m_CurrentBuffer++;
			m_CurrentBuffer = m_CurrentBuffer % m_BufferCount;
		}



		/////////////////////////////////////////////////////////////
		void DX11Swapchain::Create(const SwapchainInfo& info)
		{
			using namespace System;

			DXGI_SWAP_CHAIN_DESC desc = {};
			desc.BufferCount = info.Buffer.Count;
			desc.BufferDesc.Format = ReToDXFormat(info.Buffer.Format);
			desc.BufferDesc.Height = info.Buffer.Height;
			desc.BufferDesc.Width = info.Buffer.Width;
			desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			desc.BufferDesc.RefreshRate.Denominator = 1;
			desc.BufferDesc.RefreshRate.Numerator = 0;
			desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

			//TODO: Tearing?
			desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;

			desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			desc.Windowed = true;
			
			HWND hWnd = reinterpret_cast<const Win32WindowImpl*>(info.pWindow->GetImplementation())->GetHWND();
			desc.OutputWindow = hWnd;


			IDXGIFactory* pDXGIFactory = m_Factory->GetDXGIFactory();
			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			
			HRESULT hr = pDXGIFactory->CreateSwapChain(pD3D11Device, &desc, &m_Swapchain);
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create SwapChain. " + DXErrorString(hr));
				return;
			}
			else
			{
				m_BufferCount = info.Buffer.Count;
				m_Swapchain->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(info.Name.size()), info.Name.c_str());
			}

			CreateTextures(info);
		}



		/////////////////////////////////////////////////////////////
		void DX11Swapchain::CreateTextures(const SwapchainInfo& info)
		{
			using namespace Microsoft::WRL;
			using namespace System;

			ComPtr<ID3D11Texture2D> pTexture = nullptr;
			HRESULT hr = m_Swapchain->GetBuffer(0, IID_PPV_ARGS(&pTexture));
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not get backbuffer. " + DXErrorString(hr));
			}
			else
			{
				std::string name = info.Name + ": BackBuffer";
				pTexture->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());

				m_Texture = new DX11Texture(m_Device, pTexture.Get());
			}
		}
	}
}

#endif