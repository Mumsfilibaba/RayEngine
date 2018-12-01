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

#include "../../Include/Debug/Debug.h"
#include <vector>
#include "../../Include/DX11/DX11Factory.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "../../Include/DX11/DX11Device.h"
#include "../../Include/DX11/DX11Swapchain.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11Factory::DX11Factory(bool debugLayer)
			: m_Factory(nullptr),
			m_DebugLayer(debugLayer),
			m_References(0)
		{
			AddRef();
			Create();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11Factory::~DX11Factory()
		{
			D3DRelease_S(m_Factory);

			//LogService::GraphicsLog(nullptr);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Factory::EnumerateAdapters(AdapterList* pList) const
		{
			using namespace Microsoft::WRL;

			std::vector<AdapterDesc> adapters;
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
						adapters.push_back(AdapterDesc());
						FillAdapterDesc(i, &adapters[adapters.size() - 1], &desc);
					}
				}
			}


			(*pList) = AdapterList(static_cast<int32>(adapters.size()));
			for (int32 i = 0; i < pList->Count; i++)
				(*pList)[i] = adapters[i];
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Factory::CreateDevice(IDevice** ppDevice, const DeviceDesc* pDesc)
		{
			return ((*ppDevice) = new DX11Device(this, pDesc, m_DebugLayer)) != nullptr;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Factory::CreateSwapchain(ISwapchain** ppSwapchain, IDevice* pDevice, const SwapchainDesc* pDesc)
		{
			return ((*ppSwapchain) = new DX11Swapchain(pDevice, pDesc)) != nullptr;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Factory::CreateDeviceAndSwapchain(IDevice** ppDevice, const DeviceDesc* pDeviceDesc, ISwapchain** ppSwapchain, const SwapchainDesc* pSwapchainDesc)
		{
			IDevice* pDevice = new DX11Device(this, pDeviceDesc, m_DebugLayer);
			ISwapchain* pSwapchain = new DX11Swapchain(pDevice, pSwapchainDesc);

			(*ppDevice) = pDevice;
			(*ppSwapchain) = pSwapchain;

			return (pDevice != nullptr) && (pSwapchain != nullptr);
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Factory::SetName(const std::string& name)
		{
			m_Factory->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());
		}

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GRAPHICS_API DX11Factory::GetGraphicsApi() const
		{
			return GRAPHICS_API_D3D11;
		}

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11Factory::GetReferenceCount() const
		{
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11Factory::AddRef()
		{
			m_References++;
			return m_References;
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11Factory::Release()
		{
			m_References--;
			IObject::CounterType counter = m_References;

			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Factory::Create()
		{
			/*if (m_DebugLayer)
				LogService::GraphicsLog(new OutputLog());
			else
				LogService::GraphicsLog(new NullLog());


			if (FAILED(CreateDXGIFactory(IID_PPV_ARGS(&m_Factory))))
			{
				LogService::GraphicsLog()->Write(LOG_SEVERITY_ERROR, "DX11: Could not create factoiry");
			}*/
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Factory::FillAdapterDesc(int32 adapterID, AdapterDesc* pDesc, DXGI_ADAPTER_DESC* pDXGIDesc)
		{
			pDesc->ApiID = adapterID;
			pDesc->VendorID = pDXGIDesc->VendorId;
			pDesc->DeviceID = pDXGIDesc->DeviceId;


			constexpr int32 len = sizeof(pDXGIDesc->Description) / sizeof(WCHAR);
			char str[len];
			wcstombs(str, pDXGIDesc->Description, len);
			pDesc->ModelName = str;

			pDesc->VendorName = AdapterDesc::GetVendorString(pDXGIDesc->VendorId);


			//These are constants for D3D_FEATURE_LEVEL_11_0 the lowest level RayEngine supports
			pDesc->Flags |= ADAPTER_FLAGS_SWAPCHAIN;
			pDesc->Flags |= ADAPTER_FLAGS_TESSELATIONSHADERS;
			pDesc->Flags |= ADAPTER_FLAGS_GEOMETRYSHADER;
			pDesc->Flags |= ADAPTER_FLAGS_COMPUTE;
			pDesc->Flags |= ADAPTER_FLAGS_GRAPHICS;


			pDesc->Limits.RenderTargetCount = 8;

			pDesc->Limits.Texture1D.Width = 16384;

			pDesc->Limits.Texture2D.Width = 16384;
			pDesc->Limits.Texture2D.Height = 16384;

			pDesc->Limits.Texture3D.Width = 2048;
			pDesc->Limits.Texture3D.Height = 2048;
			pDesc->Limits.Texture3D.Depth = 2048;

			pDesc->Limits.TextureCube.Width = 16384;
			pDesc->Limits.TextureCube.Height = 16384;
		}
	}
}

#endif