#pragma once

#include "..\Defines.h"
#include "..\Types.h"

namespace RayEngine
{
	namespace Graphics
	{
		class IDevice;
		struct DeviceInfo;

		class ISwapChain;
		struct SwapChainInfo;

		struct AdapterInfo;

		class IFactory
		{
		public:
			virtual ~IFactory() {}

			//Get the information about the available graphicscards (Adapters) on the system
			virtual void EnumerateAdapters(AdapterInfo** adapters, int32& count) const = 0;

			//Create a device for interaction with the graphicscard
			virtual bool CreateDevice(IDevice** device, const DeviceInfo& deviceInfo) const = 0;
			//Create a device for interaction with the graphicscard and a swapchain for presenting images to the window
			virtual bool CreateDeviceAndSwapChain(IDevice** device, const DeviceInfo& deviceInfo,
				ISwapChain** swapChain, const DeviceInfo& swapChainInfo) const = 0;

			//Destroy Device
			virtual void DestroyDevice(IDevice** device) const = 0;
			//Destroy SwapChain
			virtual void DestroySwapChain(ISwapChain** swapChain) const = 0;
		};
	}
}