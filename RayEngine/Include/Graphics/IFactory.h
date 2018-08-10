#pragma once

#include "AdapterInfo.h"
#include "IDevice.h"
#include "ISwapchain.h"

namespace RayEngine
{
	namespace Graphics
	{
		class IFactory : public RefCounter
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