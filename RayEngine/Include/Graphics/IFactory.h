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
#include "AdapterInfo.h"
#include "..\ReferenceCounter.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class IDevice;
		struct DeviceInfo;

		class ISwapchain;
		struct SwapchainInfo;



		/////////////////////////////////////////////////////////////
		class IFactory : public ReferenceCounter
		{
			IFactory(IFactory&& other) = delete;
			IFactory(const IFactory& other) = delete;
			IFactory& operator=(IFactory&& other) = delete;
			IFactory& operator=(const IFactory& other) = delete;

		public:
			IFactory() {}
			virtual ~IFactory() {}

			//Get the information about the available graphicscards (Adapters) on the system
			virtual void EnumerateAdapters(AdapterList& list) const = 0;
			//Create a device for interaction with the graphicscard
			virtual bool CreateDevice(IDevice** ppDevice, const DeviceInfo& deviceInfo) = 0;
			//Create a swapchain for presenting images to the window
			virtual bool CreateSwapchain(ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo) = 0;
			//Create a device for interaction with the graphicscard and a swapchain for presenting images to the window
			virtual bool CreateDeviceAndSwapchain(IDevice** ppDevice, const DeviceInfo& deviceInfo, ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo) = 0;

			virtual GRAPHICS_API GetGraphicsApi() const = 0;

		public:
			static IFactory* Create(GRAPHICS_API api, bool debugLayers);
			static IFactory* Create(const std::string& name, GRAPHICS_API api, bool debugLayers);
		};
	}
}