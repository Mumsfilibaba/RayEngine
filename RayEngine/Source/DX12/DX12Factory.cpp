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
#include <cstdlib>
#include "../../Include/DX12/DX12Factory.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "../../Include/DX12/DX12Device.h"
#include "../../Include/DX12/DX12Swapchain.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12Factory::DX12Factory(bool debugLayer)
			: m_Factory(nullptr),
			m_DebugController(nullptr),
			m_References(0)
		{
			AddRef();
			Create(debugLayer);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12Factory::~DX12Factory()
		{
			D3DRelease_S(m_Factory);
			D3DRelease_S(m_DebugController);

			//LogService::GraphicsLog(nullptr);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Factory::EnumerateAdapters(AdapterList* pList) const
		{
			using namespace Microsoft::WRL;

			std::vector<AdapterDesc> adapterInfos;
			ComPtr<IDXGIAdapter1> adapter;
			ComPtr<ID3D12Device> dummyDevice;

			for (uint32 i = 0; m_Factory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; i++)
			{
				DXGI_ADAPTER_DESC1 desc = {};
				if (SUCCEEDED(adapter->GetDesc1(&desc)))
				{
					if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&dummyDevice))))
					{
						adapterInfos.push_back(AdapterDesc());
						int32 index = static_cast<int32>(adapterInfos.size() - 1);

						FillAdapterDesc(index, &adapterInfos[index], &desc);
					}
				}
			}


			*pList = AdapterList(static_cast<int32>(adapterInfos.size()));
			for (int32 i = 0; i < pList->Count; i++)
				(*pList)[i] = adapterInfos[i];
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX12Factory::CreateDevice(IDevice** ppDevice, const DeviceDesc* pDesc)
		{
			return ((*ppDevice) = new DX12Device(this, pDesc, m_DebugController != nullptr)) != nullptr;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX12Factory::CreateSwapchain(ISwapchain** ppSwapchain, IDevice* pDevice, const SwapchainDesc* pDesc)
		{
			return ((*ppSwapchain) = new DX12Swapchain(this, pDevice, pDesc)) != nullptr;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX12Factory::CreateDeviceAndSwapchain(IDevice** ppDevice, const DeviceDesc* pDeviceDesc, ISwapchain** ppSwapchain, const SwapchainDesc* pSwapchainDesc)
		{
			IDevice* pDevice = new DX12Device(this, pDeviceDesc, m_DebugController != nullptr);
			ISwapchain* pSwapchain = new DX12Swapchain(this, pDevice, pSwapchainDesc);

			(*ppDevice) = pDevice;
			(*ppSwapchain) = pSwapchain;

			return (pDevice != nullptr) && (pSwapchain != nullptr);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Factory::SetName(const std::string& name)
		{
			m_Factory->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GRAPHICS_API DX12Factory::GetGraphicsApi() const
		{
			return GRAPHICS_API_D3D12;
		}

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12Factory::GetReferenceCount() const
		{
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12Factory::AddRef()
		{
			m_References++;
			return m_References;
		}

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12Factory::Release()
		{
			m_References--;
			IObject::CounterType counter = m_References;

			if (counter < 1)
				delete this;

			return counter;
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Factory::Create(bool debugLayer)
		{
			uint32 factoryFlags = 0;
			/*if (debugLayer)
			{
				LogService::GraphicsLog(new OutputLog());

				factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
				if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_DebugController))))
				{
					LogService::GraphicsLog()->Write(LOG_SEVERITY_ERROR, "DX12: Could not retrive the Debug interface");
					return;
				}
				else
				{
					m_DebugController->EnableDebugLayer();
				}
			}
			else
			{
				LogService::GraphicsLog(new NullLog());
			}


			if (FAILED(CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&m_Factory))))
			{
				LogService::GraphicsLog()->Write(LOG_SEVERITY_ERROR, "DX12: Could not create factory");
			}*/
		}

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Factory::FillAdapterDesc(int32 adapterID, AdapterDesc* pDesc, DXGI_ADAPTER_DESC1* pDXGIDesc)
		{
			pDesc->ApiID = adapterID;
			pDesc->VendorID = pDXGIDesc->VendorId;
			pDesc->DeviceID = pDXGIDesc->DeviceId;


			constexpr int32 len = sizeof(pDXGIDesc->Description) / sizeof(WCHAR);
			char str[len];
			wcstombs(str, pDXGIDesc->Description, len);
			pDesc->ModelName = str;

			pDesc->VendorName = AdapterDesc::GetVendorString(pDXGIDesc->VendorId);


			pDesc->Flags |= ADAPTER_FLAGS_SWAPCHAIN;
			if (pDXGIDesc->Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				pDesc->Flags |= ADAPTER_FLAGS_SOFTWARE;


			//These are constants for D3D_FEATURE_LEVEL_11_0 the lowest level RayEngine supports
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