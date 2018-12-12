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

#include "RayEngine.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "Win32/Win32WindowImpl.h"
#include "DX11/DX11Swapchain.h"
#include "DX11/DX11Device.h"
#include "DX11/DX11Texture.h"
#include "DX11/DX11DepthStencilView.h"
#include "DX11/DX11RenderTargetView.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11Swapchain::DX11Swapchain(IDevice* pDevice, const SwapchainDesc* pDesc, HWND hwnd)
			: m_Device(nullptr),
			m_Swapchain(nullptr),
			m_pImmediateContext(nullptr),
			m_BackBuffer(nullptr),
			m_MSAABackBuffer(nullptr),
			m_DepthStencil(nullptr),
			m_Rtv(nullptr),
			m_Dsv(nullptr),
			m_Desc(),
			m_Flags(0),
			m_References(0),
			m_UseMSAA(false),
			m_Format(DXGI_FORMAT_UNKNOWN)
		{
			AddRef();
			m_Device = reinterpret_cast<DX11Device*>(pDevice);
			m_Device->GetD3D11Device()->GetImmediateContext(&m_pImmediateContext);

			Create(pDesc, hwnd);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11Swapchain::~DX11Swapchain()
		{
			D3DRelease_S(m_Swapchain);
			D3DRelease_S(m_pImmediateContext);
			
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
		void DX11Swapchain::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX11Device>();
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
			if (m_UseMSAA)
			{
				ID3D11Texture2D* pDst = m_BackBuffer->GetD3D11Texture2D();
				ID3D11Texture2D* pSrc = m_MSAABackBuffer->GetD3D11Texture2D();
				m_pImmediateContext->ResolveSubresource(pDst, 0, pSrc, 0, m_Format);
			}

			m_Swapchain->Present(0, 0);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Swapchain::Create(const SwapchainDesc* pDesc, HWND hwnd)
		{
			DXGI_SWAP_CHAIN_DESC desc = {};
			desc.BufferCount = pDesc->BackBuffer.Count;
			desc.BufferDesc.Format = m_Format = ReToDXFormat(pDesc->BackBuffer.Format);
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
			m_UseMSAA = (pDesc->SampleCount > 1);

			desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			desc.Windowed = true;
			
			desc.OutputWindow = hwnd;

			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			IDXGIFactory* pDXGIFactory = m_Device->GetDXGIFactory();
			HRESULT hr = pDXGIFactory->CreateSwapChain(pD3D11Device, &desc, &m_Swapchain);
			if (FAILED(hr))
			{
				LOG_ERROR("D3D11: Could not create SwapChain. " + DXErrorString(hr));
				return;
			}
			else
			{
				m_Desc = *pDesc;

				pDXGIFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);

				CreateTextures();
				CreateViews();
			}				
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Swapchain::CreateTextures()
		{
			using namespace Microsoft::WRL;

			ComPtr<ID3D11Texture2D> pTexture = nullptr;
			HRESULT hr = m_Swapchain->GetBuffer(0, IID_PPV_ARGS(&pTexture));
			if (FAILED(hr))
			{
				LOG_ERROR("D3D11: Could not get backbuffer. " + DXErrorString(hr));
			}
			else
			{
				//Create the backbuffer
				m_BackBuffer = new DX11Texture(m_Device, pTexture.Get());
				if (m_UseMSAA)
				{
					TextureDesc msaaBuffer = {};
					msaaBuffer.Flags = TEXTURE_FLAGS_RENDERTARGET;
					msaaBuffer.CpuAccess = CPU_ACCESS_FLAG_NONE;
					msaaBuffer.Width = m_Desc.Width;
					msaaBuffer.Height = m_Desc.Height;
					msaaBuffer.DepthOrArraySize = 1;
					msaaBuffer.Format = m_Desc.BackBuffer.Format;
					msaaBuffer.SampleCount = m_Desc.SampleCount;
					msaaBuffer.OptimizedColor[0] = 0.0f;
					msaaBuffer.OptimizedColor[1] = 0.0f;
					msaaBuffer.OptimizedColor[2] = 0.0f;
					msaaBuffer.OptimizedColor[3] = 1.0f;
					msaaBuffer.MipLevels = 1;
					msaaBuffer.Type = TEXTURE_TYPE_2D;
					msaaBuffer.Usage = RESOURCE_USAGE_DEFAULT;

					m_MSAABackBuffer = new DX11Texture(m_Device, nullptr, &msaaBuffer);
				}

				//Create the depthstencil
				if (m_Desc.DepthStencil.Format != FORMAT_UNKNOWN)
				{
					TextureDesc depthStencilInfo = {};
					depthStencilInfo.Flags = TEXTURE_FLAGS_DEPTH_STENCIL;
					depthStencilInfo.CpuAccess = CPU_ACCESS_FLAG_NONE;
					depthStencilInfo.Width = m_Desc.Width;
					depthStencilInfo.Height = m_Desc.Height;
					depthStencilInfo.DepthOrArraySize = 1;
					depthStencilInfo.DepthStencil.OptimizedDepth = 1.0f;
					depthStencilInfo.DepthStencil.OptimizedStencil = 0;
					depthStencilInfo.Format = m_Desc.DepthStencil.Format;
					depthStencilInfo.SampleCount = m_Desc.SampleCount;
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
			rtvInfo.Format = m_Desc.BackBuffer.Format;
			if (m_UseMSAA)
			{
				rtvInfo.ViewDimension = VIEWDIMENSION_TEXTURE2DMS;
				rtvInfo.pResource = m_MSAABackBuffer;
			}
			else
			{
				rtvInfo.ViewDimension = VIEWDIMENSION_TEXTURE2D;
				rtvInfo.pResource = m_BackBuffer;
				rtvInfo.Texture2D.MipSlice = 0;
				rtvInfo.Texture2D.PlaneSlice = 0;
			}

			m_Rtv = new DX11RenderTargetView(m_Device, &rtvInfo);


			if (m_Desc.DepthStencil.Format != FORMAT_UNKNOWN)
			{
				DepthStencilViewDesc dsvInfo = {};
				dsvInfo.Flags = DEPTH_STENCIL_VIEW_FLAGS_NONE;
				dsvInfo.pResource = m_DepthStencil;
				dsvInfo.Format = m_Desc.DepthStencil.Format;
				if (m_Desc.SampleCount > 1)
				{
					dsvInfo.ViewDimension = VIEWDIMENSION_TEXTURE2DMS;
				}
				else
				{
					dsvInfo.ViewDimension = VIEWDIMENSION_TEXTURE2D;
					dsvInfo.Texture2D.MipSlice = 0;
				}

				m_Dsv = new DX11DepthStencilView(m_Device, &dsvInfo);
			}
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Swapchain::ReleaseResources()
		{
			ReRelease_S(m_BackBuffer);
			ReRelease_S(m_MSAABackBuffer);
			ReRelease_S(m_Rtv);
			ReRelease_S(m_DepthStencil);
			ReRelease_S(m_Dsv);
		}
	}
}

#endif