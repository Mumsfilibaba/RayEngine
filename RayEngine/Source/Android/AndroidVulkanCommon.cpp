#include "..\..\Include\Vulkan\VulkCommon.h"

#if defined(RE_PLATFORM_ANDROID)
#include "AndroidAppState.h"

/////////////////////////////////////////////////////////////
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