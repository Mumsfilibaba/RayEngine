#pragma once

#include "AdapterInfo.h"
#include "IDevice.h"
#include "ISwapchain.h"

namespace RayEngine
{
	namespace Graphics
	{
		class IFactory
		{
		public:
			virtual ~IFactory() {}

			//Get the information about the available graphicscards (Adapters) on the system
			virtual void EnumerateAdapters(AdapterInfo** adapters, int32& count) const = 0;

			//Create a device for interaction with the graphicscard
			virtual bool CreateDevice(IDevice** device, const DeviceInfo& deviceInfo) const = 0;
			//Create a device for interaction with the graphicscard and a swapchain for presenting images to the window
			virtual bool CreateDeviceAndSwapchain(IDevice** device, const DeviceInfo& deviceInfo,
				ISwapchain** swapchain, const SwapchainInfo& swapchainInfo) const = 0;

			//Destroy SwapChain
			virtual void DestroySwapchain(const IDevice* const device, ISwapchain** swapchain) const = 0;

			virtual GRAPHICS_API GetGraphicsApi() const = 0;

		public:
			static IFactory* Create(GRAPHICS_API api, bool debugLayers);
		};
	}
}