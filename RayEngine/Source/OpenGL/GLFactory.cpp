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

#include "..\..\Include\OpenGL\GLFactory.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		GLFactory::GLFactory(bool debugLayer)
		{
		}



		/////////////////////////////////////////////////////////////
		GLFactory::~GLFactory()
		{
		}



		/////////////////////////////////////////////////////////////
		void GLFactory::EnumerateAdapters(AdapterList& list) const
		{
		}



		/////////////////////////////////////////////////////////////
		bool GLFactory::CreateDevice(IDevice** ppDevice, const DeviceInfo& deviceInfo)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool GLFactory::CreateSwapchain(ISwapchain** ppSwapchain, IDevice* pDevice, const SwapchainInfo& swapchainInfo)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool GLFactory::CreateDeviceAndSwapchain(IDevice** ppDevice, const DeviceInfo& deviceInfo, ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		void GLFactory::SetName(const std::string& name)
		{
			//Not relevant
		}



		/////////////////////////////////////////////////////////////
		GRAPHICS_API GLFactory::GetGraphicsApi() const
		{
			return GRAPHICS_API();
		}



		/////////////////////////////////////////////////////////////
		void GLFactory::Create(bool debugLayer)
		{
		}
	}
}