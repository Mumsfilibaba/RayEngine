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

#pragma once
#include "..\Graphics\IFactory.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX11Factory final : public IFactory
		{
			RE_IMPLEMENT_INTERFACE(DX11Factory);

		public:
			DX11Factory(bool debugLayer);
			~DX11Factory();

			IDXGIFactory* GetDXGIFactory() const;

			void EnumerateAdapters(AdapterList& list) const override final;
			
			bool CreateDevice(IDevice** ppDevice, const DeviceDesc& deviceInfo) override final;
			
			bool CreateSwapchain(ISwapchain** ppSwapchain, IDevice* pDevice, const SwapchainDesc& swapchainInfo) override final;
			
			bool CreateDeviceAndSwapchain(IDevice** ppDevice, const DeviceDesc& deviceInfo, ISwapchain** ppSwapchain, const SwapchainDesc& swapchainInfo) override final;

			void SetName(const std::string& name) override final;
			
			GRAPHICS_API GetGraphicsApi() const override final;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;

		private:
			void Create();

		private:
			IDXGIFactory* mFactory;
			
			bool m_DebugLayer;
			
			IObject::CounterType mReferences;

		private:
			static void FillAdapterInfo(int32 adapterID, AdapterDesc& info, DXGI_ADAPTER_DESC& desc);
		};
	}
}

#endif