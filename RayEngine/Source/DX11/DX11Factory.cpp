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

#include <vector>
#include "..\..\Include\DX11\DX11Factory.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"
#include "..\..\Include\DX11\DX11Swapchain.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11Factory::DX11Factory(bool debugLayer)
			: m_Factory(nullptr),
			m_DebugLayer(debugLayer),
			m_References(0)
		{
			AddRef();
			Create();
		}



		/////////////////////////////////////////////////////////////
		DX11Factory::~DX11Factory()
		{
			D3DRelease_S(m_Factory);
		}



		/////////////////////////////////////////////////////////////
		IDXGIFactory* DX11Factory::GetDXGIFactory() const
		{
			return m_Factory;
		}



		/////////////////////////////////////////////////////////////
		void DX11Factory::EnumerateAdapters(AdapterList& list) const
		{
			using namespace Microsoft::WRL;

			std::vector<AdapterInfo> infos;
			ComPtr<IDXGIAdapter> pAdapter = nullptr;
			ComPtr<ID3D11Device> pDevice = nullptr;
			ComPtr<ID3D11DeviceContext> pDeviceContext = nullptr;
			D3D_FEATURE_LEVEL featureLevel;
			D3D_FEATURE_LEVEL supportedFeatureLevel = D3D_FEATURE_LEVEL_11_0;


			for (int32 i = 0; m_Factory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; i++)
			{
				HRESULT hr = D3D11CreateDevice(pAdapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, 0, 0, &supportedFeatureLevel, 1,
					D3D11_SDK_VERSION, &pDevice, &featureLevel, &pDeviceContext);

				if (SUCCEEDED(hr))
				{
					DXGI_ADAPTER_DESC desc = {};
					if (SUCCEEDED(pAdapter->GetDesc(&desc)))
					{
						infos.push_back(AdapterInfo());
						FillAdapterInfo(i, infos[infos.size() - 1], desc);
					}
				}
			}


			list = AdapterList(static_cast<int32>(infos.size()));
			for (int32 i = 0; i < list.Count; i++)
				list[i] = infos[i];
		}



		/////////////////////////////////////////////////////////////
		bool DX11Factory::CreateDevice(IDevice** ppDevice, const DeviceInfo& deviceInfo)
		{
			return ((*ppDevice) = new DX11Device(this, deviceInfo, m_DebugLayer)) != nullptr;
		}



		/////////////////////////////////////////////////////////////
		bool DX11Factory::CreateSwapchain(ISwapchain** ppSwapchain, IDevice* pDevice, const SwapchainInfo& swapchainInfo)
		{
			return ((*ppSwapchain) = new DX11Swapchain(this, pDevice, swapchainInfo)) != nullptr;
		}



		/////////////////////////////////////////////////////////////
		bool DX11Factory::CreateDeviceAndSwapchain(IDevice** ppDevice, const DeviceInfo& deviceInfo, ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo)
		{
			IDevice* pDevice = new DX11Device(this, deviceInfo, m_DebugLayer);
			ISwapchain* pSwapchain = new DX11Swapchain(this, pDevice, swapchainInfo);

			(*ppDevice) = pDevice;
			(*ppSwapchain) = pSwapchain;

			return (pDevice != nullptr) && (pSwapchain != nullptr);
		}



		/////////////////////////////////////////////////////////////
		void DX11Factory::SetName(const std::string& name)
		{
			m_Factory->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());
		}



		/////////////////////////////////////////////////////////////
		GRAPHICS_API DX11Factory::GetGraphicsApi() const
		{
			return GRAPHICS_API_D3D11;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType DX11Factory::GetReferenceCount() const
		{
			return m_References;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType DX11Factory::AddRef()
		{
			m_References++;
			return m_References;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType DX11Factory::Release()
		{
			IObject::CounterType counter = m_References--;
			if (m_References < 1)
				delete this;

			return counter;
		}



		/////////////////////////////////////////////////////////////
		void DX11Factory::Create()
		{
			if (FAILED(CreateDXGIFactory(IID_PPV_ARGS(&m_Factory))))
			{
				return;
			}
		}



		/////////////////////////////////////////////////////////////
		void DX11Factory::FillAdapterInfo(int32 adapterID, AdapterInfo& info, DXGI_ADAPTER_DESC& desc)
		{
			info.ApiID = adapterID;
			info.VendorID = desc.VendorId;
			info.DeviceID = desc.DeviceId;


			constexpr int32 len = sizeof(desc.Description) / sizeof(WCHAR);
			char str[len];
			wcstombs(str, desc.Description, len);
			info.ModelName = str;

			info.VendorName = AdapterInfo::GetVendorString(desc.VendorId);


			//These are constants for D3D_FEATURE_LEVEL_11_0 the lowest level RayEngine supports
			info.Flags |= ADAPTER_FLAGS_SWAPCHAIN;
			info.Flags |= ADAPTER_FLAGS_TESSELATIONSHADERS;
			info.Flags |= ADAPTER_FLAGS_GEOMETRYSHADER;
			info.Flags |= ADAPTER_FLAGS_COMPUTE;
			info.Flags |= ADAPTER_FLAGS_GRAPHICS;


			info.Limits.RenderTargetCount = 8;

			info.Limits.Texture1D.Width = 16384;

			info.Limits.Texture2D.Width = 16384;
			info.Limits.Texture2D.Height = 16384;

			info.Limits.Texture3D.Width = 2048;
			info.Limits.Texture3D.Height = 2048;
			info.Limits.Texture3D.Depth = 2048;

			info.Limits.TextureCube.Width = 16384;
			info.Limits.TextureCube.Height = 16384;
		}
	}
}

#endif