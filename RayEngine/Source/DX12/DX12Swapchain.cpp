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

#include "..\..\Include\DX12\DX12Swapchain.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX12\DX12Factory.h"
#include "..\..\Include\DX12\DX12DeviceContext.h"
#include "..\..\Include\Win32\Win32WindowImpl.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Swapchain::DX12Swapchain(IFactory* pFactory, const SwapchainInfo& info)
			: m_Factory(nullptr),
			m_Context(nullptr),
			m_Swapchain(nullptr),
			m_CurrentBuffer(0),
			m_Textures()
		{
			AddRef();

			m_Device = QueryDX12Device(info.pDevice);
			m_Device->GetImmediateContext(reinterpret_cast<IDeviceContext**>(&m_Context));
			m_Factory = reinterpret_cast<DX12Factory*>(pFactory->QueryReference());

			Create(info);
		}



		/////////////////////////////////////////////////////////////
		DX12Swapchain::~DX12Swapchain()
		{
			D3DRelease_S(m_Swapchain);
			
			ReRelease_S(m_Factory);
			ReRelease_S(m_Device);
			ReRelease_S(m_Context);

			for (int32 i = 0; i < m_Textures.size(); i++)
			{
				ReRelease_S(m_Textures[i]);
			}
		}



		/////////////////////////////////////////////////////////////
		int32 DX12Swapchain::GetCurrentBuffer() const
		{
			return m_CurrentBuffer;
		}



		/////////////////////////////////////////////////////////////
		ITexture* DX12Swapchain::GetBuffer(int32 index)
		{
			return m_Textures[index];
		}



		/////////////////////////////////////////////////////////////
		const ITexture* DX12Swapchain::GetBuffer(int32 index) const
		{
			return m_Textures[index];
		}



		/////////////////////////////////////////////////////////////
		void DX12Swapchain::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryDX12Device(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX12Swapchain::QueryFactory(IFactory** ppFactory) const
		{
			(*ppFactory) = reinterpret_cast<DX12Factory*>(m_Factory->QueryReference());
		}



		/////////////////////////////////////////////////////////////
		void DX12Swapchain::Present() const
		{
			m_Swapchain->Present(0, 0);

			m_CurrentBuffer++;
			m_CurrentBuffer = m_CurrentBuffer % m_Textures.size();
		}



		/////////////////////////////////////////////////////////////
		void DX12Swapchain::Create(const SwapchainInfo& info)
		{
			DXGI_SWAP_CHAIN_DESC1 desc = {};
			desc.BufferCount = info.Buffer.Count;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.Width = info.Buffer.Width;
			desc.Height = info.Buffer.Height;
			desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			desc.SampleDesc.Count = 1;
			
			//TODO: Engine swap effect
			desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			
			//TODO: Tearing?
			desc.Flags = 0;

			//COMMANDQUEUE??
			HWND hWnd = reinterpret_cast<const System::Win32WindowImpl*>(info.pWindow->GetImplementation())->GetHWND();
			IDXGIFactory5* pDXGIFactory = m_Factory->GetDXGIFactory();
			ID3D12CommandQueue* pD3D12queue = m_Context->GetD3D12CommandQueue();
			

			if (FAILED(pDXGIFactory->CreateSwapChainForHwnd(pD3D12queue, hWnd, &desc, nullptr, nullptr, &m_Swapchain)))
			{
				return;
			}
			else
			{
				m_Swapchain->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(info.Name.size()), info.Name.c_str());
				
				pDXGIFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);
				
				CreateTextures(info);
			}
		}



		/////////////////////////////////////////////////////////////
		void DX12Swapchain::CreateTextures(const SwapchainInfo& info)
		{
			using namespace System;
			using namespace Microsoft::WRL;

			m_Textures.resize(info.Buffer.Count);


			ComPtr<ID3D12Resource> buffer = nullptr;
			for (int32 i = 0; i < info.Buffer.Count; i++)
			{
				HRESULT hr = m_Swapchain->GetBuffer(i, IID_PPV_ARGS(&buffer));
				if (FAILED(hr))
				{
					m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D12: Could not get baqckbuffer. " + DXErrorString(hr));
					return;
				}
				else
				{
					D3D12SetName(buffer.Get(), info.Name + std::to_string(i));
					
					m_Textures[i] = new DX12Texture(m_Device, buffer.Get());
				}
			}
		}
	}
}

#endif