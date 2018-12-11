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

#if defined(RE_PLATFORM_ANDROID)
#include "..\..\Include\Vulkan\VulkCommon.h"
#include "AndroidAppState.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VkResult VulkanCreateSwapchainSurface(VkInstance instance, VkSurfaceKHR* surface, const RayEngine::System::IWindowImpl* window)
{
	using namespace RayEngine;
	//TODO: ANativeWindow shoule be in WindowImpl
	AndroidAppState* state = reinterpret_cast<AndroidAppState*>(GetNativeActivity()->instance);
	
	VkAndroidSurfaceCreateInfoKHR info = {};
	info.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
	info.flags = 0;
	info.pNext = nullptr;
	info.window = state->NativeWindow;

	auto vkCreateAndroidSurfaceKHR = reinterpret_cast<PFN_vkCreateAndroidSurfaceKHR>(vkGetInstanceProcAddr(instance, "vkCreateAndroidSurfaceKHR"));
	VkResult result = vkCreateAndroidSurfaceKHR(instance, &info, nullptr, surface);
	return result;
}

#endif