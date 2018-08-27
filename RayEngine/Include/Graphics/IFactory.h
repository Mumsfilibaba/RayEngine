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


			/*////////////////////////////////////////////////////////////

				Retrives a list of all the adapters (physical devices)
				that is found on the system by the chosen api.

				list - A AdapterList-structure that contains all info
				about the adpaters in the system.

			////////////////////////////////////////////////////////////*/
			virtual void EnumerateAdapters(AdapterList& list) const = 0;
			
			
			/*////////////////////////////////////////////////////////////

				Creates a new logical device.

				ppDevice - A valid pointer to a IDevice interface.

				deviceInfo - Structure that contains information about
				the device to be created.

			////////////////////////////////////////////////////////////*/
			virtual bool CreateDevice(IDevice** ppDevice, const DeviceInfo& deviceInfo) = 0;
			
			
			/*////////////////////////////////////////////////////////////

				Creates a new swapchain.

				ppSwapchain - A valid pointer to a ISwapchain interface.

				ppDevice - A valid pointer to a IDevice interface to 
				create the swapchain with.

				swapchainInfo - Structure that contains information about
				the swapchain to be created.

			////////////////////////////////////////////////////////////*/
			virtual bool CreateSwapchain(ISwapchain** ppSwapchain, IDevice* pDevice, const SwapchainInfo& swapchainInfo) = 0;


			/*////////////////////////////////////////////////////////////

				Creates a new device and swapchain.

				ppDevice - A pointer to a IDevice interface. Can be
				nullptr and the result is the same as a call to 
				CreateSwapchain.

				deviceInfo - Structure that contains information about
				the device to be created.

				ppSwapchain - A valid pointer to a ISwapchain interface.
				Can be nullptr and the result is the same as a call to
				CreateDevice.

				swapchainInfo - Structure that contains information about
				the swapchain to be created.

			////////////////////////////////////////////////////////////*/
			virtual bool CreateDeviceAndSwapchain(IDevice** ppDevice, const DeviceInfo& deviceInfo, ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo) = 0;


			/*////////////////////////////////////////////////////////////
				Returns what the underlying api is.
			////////////////////////////////////////////////////////////*/
			virtual GRAPHICS_API GetGraphicsApi() const = 0;


			/*////////////////////////////////////////////////////////////

				Sets the name of the object.

				name - A new name of the object.

			////////////////////////////////////////////////////////////*/
			virtual void SetName(const std::string& name) = 0;

		public:

			/*////////////////////////////////////////////////////////////

				Creates a new factory.

				api - The api to use.

				debugLayers - A boolean turning the debuglayer on/off

				Returns a pointer to a IFactory interface. Can be 
				nullptr if an error occured during the call

			////////////////////////////////////////////////////////////*/
			static IFactory* Create(GRAPHICS_API api, bool debugLayers);


			/*////////////////////////////////////////////////////////////

				Creates a new factory.

				name - Set a name for the factory

				api - The api to use.

				debugLayers - A boolean turning the debuglayer on/off

				Returns a pointer to a IFactory interface. Can be
				nullptr if an error occured during the call

			////////////////////////////////////////////////////////////*/
			static IFactory* Create(const std::string& name, GRAPHICS_API api, bool debugLayers);
		};
	}
}