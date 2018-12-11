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
#include "DX12/DX12Swapchain.h"
#include "DX12/DX12DepthStencilView.h"
#include "DX12/DX12RenderTargetView.h"
#include "DX12/DX12DeviceContext.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12Swapchain::DX12Swapchain(IDevice* pDevice, const SwapchainDesc* pDesc, HWND hwnd)
			: m_Device(nullptr),
			m_Context(nullptr),
			m_Swapchain(nullptr),
			m_MSAABackBuffer(nullptr),
			m_DepthStencil(nullptr),
			m_DSV(nullptr),
			m_Textures(),
			m_RTVs(),
			m_Desc(),
			m_Format(DXGI_FORMAT_UNKNOWN),
			m_CurrentBuffer(0),
			m_References(0),
			m_UseMSAA(false)
		{
			AddRef();

			m_Device = reinterpret_cast<DX12Device*>(pDevice);
			m_Context = m_Device->GetDX12ImmediateContext();

			Create(pDesc, hwnd);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12Swapchain::~DX12Swapchain()
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Swapchain::Resize(int32 width, int32 height)
		{
			//TODO: Implement this 
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Swapchain::SetName(const std::string& name)
		{
			m_Swapchain->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());
			
			for (int32 i = 0; i < m_Desc.BackBuffer.Count; i++)
				m_Textures[i]->SetName(name + ": BackBuffer(" + std::to_string(i) + ')');

			if (m_MSAABackBuffer != nullptr)
				m_MSAABackBuffer->SetName(name + ": MSAA BackBuffer");

			if (m_DepthStencil != nullptr)
				m_DepthStencil->SetName(name + ": DepthStencil");
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Swapchain::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX12Device>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Swapchain::GetDesc(SwapchainDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12Swapchain::GetReferenceCount() const
		{
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12Swapchain::AddRef()
		{
			return ++m_References;
		}

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12Swapchain::Release()
		{
			IObject::CounterType counter = --m_References;
			if (counter < 1)
				delete this;

			return counter;
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Swapchain::Present() const
		{
			m_Context->TransitionResource(GetCurrentDX12Buffer(), D3D12_RESOURCE_STATE_PRESENT, 0);
			m_Context->Flush();

			m_Swapchain->Present(0, 0);

			m_CurrentBuffer++;
			m_CurrentBuffer = m_CurrentBuffer % m_Textures.size();
		}

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Swapchain::Create(const SwapchainDesc* pDesc, HWND hwnd)
		{
			DXGI_SWAP_CHAIN_DESC1 desc = {};
			desc.BufferCount = pDesc->BackBuffer.Count;
			desc.Format = ReToDXFormat(pDesc->BackBuffer.Format);
			desc.Width = pDesc->Width;
			desc.Height = pDesc->Height;
			desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			
			m_UseMSAA = (pDesc->SampleCount > 1);

			//TODO: Tearing?
			desc.Flags = 0;

			IDXGIFactory5* pDXGIFactory = m_Device->GetDXGIFactory();
			ID3D12CommandQueue* pD3D12queue = m_Context->GetD3D12CommandQueue();
			
			Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain = nullptr;
			HRESULT hr = pDXGIFactory->CreateSwapChainForHwnd(pD3D12queue, hwnd, &desc, nullptr, nullptr, &swapChain);
			if (FAILED(hr))
			{
				LOG_ERROR("D3D12: Could not create swapchain. " + DXErrorString(hr));
				return;
			}
			else
			{
				if (FAILED(swapChain.As(&m_Swapchain)))
				{
					LOG_ERROR("D3D12: Could not retrive IDXGISwapChain3 interface");
					return;
				}

				m_Desc = *pDesc;
				m_CurrentBuffer = m_Swapchain->GetCurrentBackBufferIndex();

				pDXGIFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
				
				CreateTextures();
				CreateViews();

				SetName(m_Desc.Name);
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Swapchain::CreateTextures()
		{
			using namespace Microsoft::WRL;

			//Get buffers from swapchain
			ComPtr<ID3D12Resource> buffer = nullptr;
			m_Textures.resize(m_Desc.BackBuffer.Count);
			for (int32 i = 0; i < m_Desc.BackBuffer.Count; i++)
			{
				HRESULT hr = m_Swapchain->GetBuffer(i, IID_PPV_ARGS(&buffer));
				if (FAILED(hr))
				{
					LOG_ERROR("D3D12: Could not get backbuffer. " + DXErrorString(hr));
					return;
				}
				else
				{
					std::string name = m_Desc.Name + ": BackBuffer(" + std::to_string(i) + ')';
					D3D12SetName(buffer.Get(), name);

					m_Textures[i] = new DX12Texture(m_Device, buffer.Get());
				}
			}

			//Create buffer for MSAA
			if (m_UseMSAA)
			{
				TextureDesc msaaBuffer = {};
				msaaBuffer.Name = m_Desc.Name + ": MSAA Backbuffer";
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

				m_MSAABackBuffer = new DX12Texture(m_Device, nullptr, &msaaBuffer);
			}

			//Create the depthstencil if requested
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
				depthStencilInfo.SampleCount = m_Desc.SampleCount;
				depthStencilInfo.MipLevels = 1;
				depthStencilInfo.Type = TEXTURE_TYPE_2D;
				depthStencilInfo.Usage = RESOURCE_USAGE_DEFAULT;

				m_DepthStencil = new DX12Texture(m_Device, nullptr, &depthStencilInfo);
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Swapchain::CreateViews()
		{
			//Set the amount of buffers needed
			m_RTVs.resize(m_Desc.BackBuffer.Count);

			//Create rendertarget views
			RenderTargetViewDesc rtvInfo = {};
			rtvInfo.Name = m_Desc.Name + ": BackBuffer RTV";
			rtvInfo.Format = m_Desc.BackBuffer.Format;
			if (m_UseMSAA)
			{
				rtvInfo.ViewDimension = VIEWDIMENSION_TEXTURE2DMS;
				rtvInfo.pResource = m_MSAABackBuffer;

				m_RTVs[0] = new DX12RenderTargetView(m_Device, &rtvInfo);
			}
			else
			{
				rtvInfo.ViewDimension = VIEWDIMENSION_TEXTURE2D;
				rtvInfo.Texture2D.MipSlice = 0;
				rtvInfo.Texture2D.PlaneSlice = 0;

				for (int32 i = 0; i < m_Desc.BackBuffer.Count; i++)
				{
					rtvInfo.pResource = m_Textures[i];
					m_RTVs[i] = new DX12RenderTargetView(m_Device, &rtvInfo);
				}
			}

			//Create depthstencilview
			if (m_Desc.DepthStencil.Format != FORMAT_UNKNOWN)
			{
				DepthStencilViewDesc dsvInfo = {};
				dsvInfo.Name = m_Desc.Name + ": DepthStencil DSV";
				dsvInfo.Flags = DEPTH_STENCIL_VIEW_FLAGS_NONE;
				dsvInfo.pResource = m_DepthStencil;
				dsvInfo.Format = m_Desc.DepthStencil.Format;
				if (m_UseMSAA)
				{
					dsvInfo.ViewDimension = VIEWDIMENSION_TEXTURE2DMS;
				}
				else
				{
					dsvInfo.ViewDimension = VIEWDIMENSION_TEXTURE2D;
					dsvInfo.Texture2D.MipSlice = 0;
				}

				m_DSV = new DX12DepthStencilView(m_Device, &dsvInfo);
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Swapchain::ReleaseResources()
		{
			ReRelease_S(m_MSAABackBuffer);
			ReRelease_S(m_DepthStencil);
			ReRelease_S(m_DSV);

			for (int32 i = 0; i < static_cast<int32>(m_Textures.size()); i++)
			{
				ReRelease_S(m_Textures[i]);
				ReRelease_S(m_RTVs[i]);
			}
		}
	}
}

#endif