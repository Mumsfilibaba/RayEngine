#pragma once

#include "AdapterInfo.h"
#include "IDevice.h"
#include "ISwapchain.h"
#include "IShaderCompiler.h"

namespace RayEngine
{
	namespace Graphics
	{
		class IFactory
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
			virtual bool CreateDevice(IDevice** device, const DeviceInfo& deviceInfo) const = 0;
			//Create a swapchain for presenting images to the window
			virtual bool CreateSwapchain(ISwapchain** swapchain, const SwapchainInfo& swapchainInfo) const = 0;
			//Create a device for interaction with the graphicscard and a swapchain for presenting images to the window
			virtual bool CreateDeviceAndSwapchain(IDevice** device, const DeviceInfo& deviceInfo,
				ISwapchain** swapchain, const SwapchainInfo& swapchainInfo) const = 0;
			//Create a object for creation of shaders
			virtual bool CreateShaderCompiler(IShaderCompiler** compiler) const = 0;

			virtual GRAPHICS_API GetGraphicsApi() const = 0;

		public:
			static IFactory* Create(GRAPHICS_API api, bool debugLayers);
		};
	}
}