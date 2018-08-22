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
#include "DX12Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12Factory final : public IFactory
		{
		public:
			DX12Factory(const DX12Factory& other) = delete;
			DX12Factory& operator=(const DX12Factory& other) = delete;
			DX12Factory(DX12Factory&& other) = delete;
			DX12Factory& operator=(DX12Factory&& other) = delete;

		public:
			DX12Factory(const std::string& name, bool debugLayer);
			~DX12Factory();

			IDXGIFactory5* GetDXGIFactory() const;
			
			void EnumerateAdapters(AdapterList& list) const override final;
			bool CreateDevice(IDevice** ppDevice, const DeviceInfo& deviceInfo) override final;
			bool CreateSwapchain(ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo) override final;
			bool CreateDeviceAndSwapchain(IDevice** ppDevice, const DeviceInfo& deviceInfo, ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo) override final;

			GRAPHICS_API GetGraphicsApi() const override final;

		private:
			void Create(const std::string& name, bool debugLayer);

		private:
			ID3D12Debug* m_DebugController;
			IDXGIFactory5* m_Factory;

		private:
			static void FillAdapterInfo(int32 adapterID, AdapterInfo& info, DXGI_ADAPTER_DESC1& desc);
		};
	}
}

#endif