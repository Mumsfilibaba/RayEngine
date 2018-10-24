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
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12Swapchain::DX12Swapchain(IFactory* pFactory, IDevice* pDevice, const SwapchainDesc* pDesc)
			: m_Factory(nullptr),
			m_Context(nullptr),
			m_Swapchain(nullptr),
			m_CurrentBuffer(0),
			m_Textures(),
			m_Desc(),
			m_References(0)
		{
			AddRef();

			m_Device = pDevice->QueryReference<DX12Device>();
			m_Device->GetImmediateContext(reinterpret_cast<IDeviceContext**>(&m_Context));

			m_Factory = pFactory->QueryReference<DX12Factory>();

			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12Swapchain::~DX12Swapchain()
		{
			D3DRelease_S(m_Swapchain);
			
			ReRelease_S(m_Factory);
			ReRelease_S(m_Device);
			ReRelease_S(m_Context);

			for (int32 i = 0; i < static_cast<int32>(m_Textures.size()); i++)
			{
				ReRelease_S(m_Textures[i]);
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Swapchain::Resize(int32 width, int32 height)
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Swapchain::SetName(const std::string& name)
		{
			m_Swapchain->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Swapchain::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX12Device>();
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Swapchain::QueryFactory(IFactory** ppFactory) const
		{
			(*ppFactory) = m_Factory->QueryReference<DX12Factory>();
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
			m_References++;
			return m_References;
		}

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12Swapchain::Release()
		{
			m_References--;
			IObject::CounterType counter = m_References;

			if (counter < 1)
				delete this;

			return counter;
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Swapchain::Present() const
		{
			m_Swapchain->Present(0, 0);

			m_CurrentBuffer++;
			m_CurrentBuffer = m_CurrentBuffer % m_Textures.size();
		}

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Swapchain::Create(const SwapchainDesc* pDesc)
		{
			using namespace System;

			DXGI_SWAP_CHAIN_DESC1 desc = {};
			desc.BufferCount = pDesc->BackBuffer.Count;
			desc.Format = ReToDXFormat(pDesc->BackBuffer.Format);
			desc.Width = pDesc->Width;
			desc.Height = pDesc->Height;
			desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			desc.SampleDesc.Count = 1;
			
			//TODO: Engine swap effect
			desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			
			//TODO: Tearing?
			desc.Flags = 0;


			IDXGIFactory5* pDXGIFactory = m_Factory->GetDXGIFactory();
			ID3D12CommandQueue* pD3D12queue = m_Context->GetD3D12CommandQueue();
			
			HRESULT hr = pDXGIFactory->CreateSwapChainForHwnd(pD3D12queue, pDesc->WindowHandle, &desc, nullptr, nullptr, &m_Swapchain);
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D12 : Could not create swapchain. " + DXErrorString(hr));
				return;
			}
			else
			{
				m_Desc = *pDesc;

				m_Swapchain->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(pDesc->Name.size()), pDesc->Name.c_str());
				
				pDXGIFactory->MakeWindowAssociation(pDesc->WindowHandle, DXGI_MWA_NO_ALT_ENTER);
				
				CreateTextures(pDesc);
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Swapchain::CreateTextures(const SwapchainDesc* pDesc)
		{
			using namespace System;
			using namespace Microsoft::WRL;

			m_Textures.resize(pDesc->BackBuffer.Count);


			ComPtr<ID3D12Resource> buffer = nullptr;
			for (int32 i = 0; i < pDesc->BackBuffer.Count; i++)
			{
				HRESULT hr = m_Swapchain->GetBuffer(i, IID_PPV_ARGS(&buffer));
				if (FAILED(hr))
				{
					m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D12: Could not get baqckbuffer. " + DXErrorString(hr));
					return;
				}
				else
				{
					D3D12SetName(buffer.Get(), pDesc->Name + std::to_string(i));
					
					m_Textures[i] = new DX12Texture(m_Device, buffer.Get());
				}
			}
		}
	}
}

#endif