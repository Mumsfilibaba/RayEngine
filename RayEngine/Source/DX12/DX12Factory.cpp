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

#include "..\..\Include\DX12\DX12Factory.h"

#if defined(RE_PLATFORM_WINDOWS)
#include <vector>
#include <cstdlib>
#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12Swapchain.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Factory::DX12Factory(const std::string& name, bool debugLayer)
			: m_Factory(nullptr),
			m_DebugController(nullptr)
		{
			AddRef();
			Create(name, debugLayer);
		}



		/////////////////////////////////////////////////////////////
		DX12Factory::~DX12Factory()
		{
			D3DRelease_S(m_Factory);
			D3DRelease_S(m_DebugController);
		}



		/////////////////////////////////////////////////////////////
		IDXGIFactory5* DX12Factory::GetDXGIFactory() const
		{
			return m_Factory;
		}



		/////////////////////////////////////////////////////////////
		void DX12Factory::EnumerateAdapters(AdapterList& list) const
		{
			using namespace Microsoft::WRL;

			std::vector<AdapterInfo> adapterInfos;
			ComPtr<IDXGIAdapter1> adapter;
			ComPtr<ID3D12Device> dummyDevice;

			for (uint32 i = 0; m_Factory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; i++)
			{
				DXGI_ADAPTER_DESC1 desc = {};
				if (SUCCEEDED(adapter->GetDesc1(&desc)))
				{
					if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&dummyDevice))))
					{
						adapterInfos.push_back(AdapterInfo());
						int32 index = static_cast<int32>(adapterInfos.size() - 1);

						FillAdapterInfo(index, adapterInfos[index], desc);
					}
				}
			}


			list = AdapterList(static_cast<int32>(adapterInfos.size()));
			for (int32 i = 0; i < list.Count; i++)
				list[i] = adapterInfos[i];
		}



		/////////////////////////////////////////////////////////////
		bool DX12Factory::CreateDevice(IDevice** ppDevice, const DeviceInfo& deviceInfo)
		{
			return (*ppDevice) = new DX12Device(this, deviceInfo, m_DebugController != nullptr);
		}



		/////////////////////////////////////////////////////////////
		bool DX12Factory::CreateSwapchain(ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo)
		{
			return (*ppSwapchain) = new DX12Swapchain(this, swapchainInfo);
		}



		/////////////////////////////////////////////////////////////
		bool DX12Factory::CreateDeviceAndSwapchain(IDevice** ppDevice, const DeviceInfo& deviceInfo, ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo)
		{
			DX12Device* pDX12Device = new DX12Device(this, deviceInfo, (m_DebugController != nullptr));
			return (pDX12Device != nullptr);
		}



		/////////////////////////////////////////////////////////////
		GRAPHICS_API DX12Factory::GetGraphicsApi() const
		{
			return GRAPHICS_API_D3D12;
		}



		/////////////////////////////////////////////////////////////
		void DX12Factory::Create(const std::string& name, bool debugLayer)
		{
			uint32 factoryFlags = 0;
			if (debugLayer)
			{
				factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
				if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_DebugController))))
				{
					return;
				}
				else
				{
					m_DebugController->EnableDebugLayer();
				}
			}


			if (FAILED(CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&m_Factory))))
			{
				return;
			}
			else
			{
				m_Factory->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());
			}
		}



		/////////////////////////////////////////////////////////////
		void DX12Factory::FillAdapterInfo(int32 adapterID, AdapterInfo& info, DXGI_ADAPTER_DESC1& desc)
		{
			info.ApiID = adapterID;
			info.VendorID = desc.VendorId;
			info.DeviceID = desc.DeviceId;


			constexpr int32 len = sizeof(desc.Description) / sizeof(WCHAR);
			char str[len];
			wcstombs(str, desc.Description, len);
			info.ModelName = str;

			info.VendorName = AdapterInfo::GetVendorString(desc.VendorId);


			info.Flags |= ADAPTER_FLAGS_SWAPCHAIN;
			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				info.Flags |= ADAPTER_FLAGS_SOFTWARE;


			//These are constants for D3D_FEATURE_LEVEL_11_0 the lowest level RayEngine supports
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