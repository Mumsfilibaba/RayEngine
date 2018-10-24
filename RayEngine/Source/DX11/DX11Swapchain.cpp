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
#include "..\..\Include\DX11\DX11DepthStencilView.h"
#include "..\..\Include\DX11\DX11RenderTargetView.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11Swapchain::DX11Swapchain(IFactory* pFactory, IDevice* pDevice, const SwapchainDesc* pDesc)
			: m_Device(nullptr),
			m_Factory(nullptr),
			m_Swapchain(nullptr),
			m_BackBuffer(nullptr),
			m_DepthStencil(nullptr),
			m_Rtv(nullptr),
			m_Dsv(nullptr),
			m_Desc(),
			m_Flags(0),
			m_References(0)
		{
			AddRef();
			m_Factory = reinterpret_cast<DX11Factory*>(pFactory);
			m_Device = reinterpret_cast<DX11Device*>(pDevice);

			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11Swapchain::~DX11Swapchain()
		{
			D3DRelease_S(m_Swapchain);
			
			ReleaseResources();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Swapchain::Resize(int32 width, int32 height)
		{
			if (width < 1 || height < 1)
				return;

			ReleaseResources();

			m_Swapchain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, m_Flags);

			CreateTextures();
			CreateViews();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Swapchain::SetName(const std::string& name)
		{
			m_Swapchain->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Swapchain::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX11Device>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Swapchain::QueryFactory(IFactory** ppFactory) const
		{
			(*ppFactory) = m_Factory->QueryReference<DX11Factory>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Swapchain::GetDesc(SwapchainDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11Swapchain::GetReferenceCount() const
		{
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11Swapchain::AddRef()
		{
			m_References++;
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11Swapchain::Release()
		{
			m_References--;
			IObject::CounterType counter = m_References;

			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Swapchain::Present() const
		{
			m_Swapchain->Present(0, 0);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Swapchain::Create(const SwapchainDesc* pDesc)
		{
			using namespace System;

			DXGI_SWAP_CHAIN_DESC desc = {};
			desc.BufferCount = pDesc->BackBuffer.Count;
			desc.BufferDesc.Format = ReToDXFormat(pDesc->BackBuffer.Format);
			desc.BufferDesc.Height = pDesc->Height;
			desc.BufferDesc.Width = pDesc->Width;
			desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			desc.BufferDesc.RefreshRate.Denominator = 1;
			desc.BufferDesc.RefreshRate.Numerator = 0;
			desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

			//TODO: Tearing?
			m_Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			desc.Flags = m_Flags;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;

			desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			desc.Windowed = true;
			
			desc.OutputWindow = pDesc->WindowHandle;


			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			IDXGIFactory* pDXGIFactory = m_Factory->GetDXGIFactory();
			HRESULT hr = pDXGIFactory->CreateSwapChain(pD3D11Device, &desc, &m_Swapchain);
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create SwapChain. " + DXErrorString(hr));
				return;
			}
			else
			{
				m_Desc = *pDesc;

				m_Swapchain->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(pDesc->Name.size()), pDesc->Name.c_str());

				pDXGIFactory->MakeWindowAssociation(pDesc->WindowHandle, DXGI_MWA_NO_ALT_ENTER);
			}				

			CreateTextures();
			CreateViews();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Swapchain::CreateTextures()
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
				std::string name = m_Desc.Name + ": BackBuffer";
				pTexture->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());

				m_BackBuffer = new DX11Texture(m_Device, pTexture.Get());


				if (m_Desc.DepthStencil.Format != FORMAT_UNKNOWN)
				{
					TextureDesc depthStencilInfo = {};
					depthStencilInfo.Name = m_Desc.Name + ": DepthStencil";
					depthStencilInfo.Flags = TEXTURE_FLAGS_DEPTH_STENCIL;
					depthStencilInfo.CpuAccess = CPU_ACCESS_FLAG_NONE;
					depthStencilInfo.Width = m_Desc.Width;
					depthStencilInfo.Height = m_Desc.Height;
					depthStencilInfo.DepthOrArraySize = 1;
					depthStencilInfo.DepthStencil.OptimizedDepth = 1.0f;
					depthStencilInfo.DepthStencil.OptimizedStencil = 0;
					depthStencilInfo.Format = m_Desc.DepthStencil.Format;
					depthStencilInfo.MipLevels = 1;
					depthStencilInfo.Type = TEXTURE_TYPE_2D;
					depthStencilInfo.Usage = RESOURCE_USAGE_DEFAULT;

					m_DepthStencil = new DX11Texture(m_Device, nullptr, &depthStencilInfo);
				}
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Swapchain::CreateViews()
		{
			RenderTargetViewDesc rtvInfo = {};
			rtvInfo.Name = m_Desc.Name + ": BackBuffer RTV";
			rtvInfo.Format = m_Desc.BackBuffer.Format;
			rtvInfo.pResource = m_BackBuffer;
			rtvInfo.ViewDimension = VIEWDIMENSION_TEXTURE2D;
			rtvInfo.Texture2D.MipSlice = 0;
			rtvInfo.Texture2D.PlaneSlice = 0;

			m_Rtv = new DX11RenderTargetView(m_Device, &rtvInfo);


			if (m_Desc.DepthStencil.Format != FORMAT_UNKNOWN)
			{
				DepthStencilViewDesc dsvInfo = {};
				dsvInfo.Name = m_Desc.Name + ": DepthStencil DSV";
				dsvInfo.Flags = DEPTH_STENCIL_VIEW_FLAGS_NONE;
				dsvInfo.pResource = m_DepthStencil;
				dsvInfo.Format = m_Desc.DepthStencil.Format;
				dsvInfo.ViewDimension = VIEWDIMENSION_TEXTURE2D;
				dsvInfo.Texture2D.MipSlice = 0;

				m_Dsv = new DX11DepthStencilView(m_Device, &dsvInfo);
			}
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Swapchain::ReleaseResources()
		{
			ReRelease_S(m_BackBuffer);
			ReRelease_S(m_Rtv);
			ReRelease_S(m_DepthStencil);
			ReRelease_S(m_Dsv);
		}
	}
}

#endif