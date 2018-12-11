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

#include "RayEngine.h"
#include "../../Include/Vulkan/VulkCommon.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "../../Include/Win32/Win32WindowImpl.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VkResult VulkanCreateSwapchainSurface(VkInstance instance, VkSurfaceKHR* surface, const NativeWindowHandle& hwnd)
		{
			using namespace RayEngine;

			VkWin32SurfaceCreateInfoKHR info = {};
			info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			info.flags = 0;
			info.pNext = nullptr;
			info.hwnd = hwnd;
			info.hinstance = static_cast<HINSTANCE>(GetModuleHandle(nullptr));;

			PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR = reinterpret_cast<PFN_vkCreateWin32SurfaceKHR>(vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR"));
			VkResult result = vkCreateWin32SurfaceKHR(instance, &info, nullptr, surface);

			return result;
		}
	}
}

#endif