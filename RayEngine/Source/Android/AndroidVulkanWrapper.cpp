#include "..\..\Include\Android\AndroidVulkanWrapper.h"

/////////////////////////////////////////////////////////////
PFN_vkCreateInstance vkCreateInstance = nullptr;
PFN_vkDestroyInstance vkDestroyInstance = nullptr;
PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties = nullptr;
PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties = nullptr;
PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices = nullptr;
PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = nullptr;
PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures = nullptr;
PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties = nullptr;
PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties = nullptr;
PFN_vkCreateDevice vkCreateDevice = nullptr;
PFN_vkDestroyDevice vkDestroyDevice = nullptr;
PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR = nullptr;
PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR = nullptr;
PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR = nullptr;
PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties = nullptr;
PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR = nullptr;
PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR = nullptr;
PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR = nullptr;
PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR = nullptr;



#if defined(RE_PLATFORM_ANDROID)
#include <dlfcn.h>

#define VK_LOADFUNCTION(function) (function = reinterpret_cast<PFN_##function>(dlsym(libVulkan, #function)))

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	bool InitializeVulkan()
	{
		void* libVulkan = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
		if (libVulkan == nullptr)
			return false;

		VK_LOADFUNCTION(vkCreateInstance);
		VK_LOADFUNCTION(vkDestroyInstance);
		VK_LOADFUNCTION(vkEnumerateInstanceExtensionProperties);
		VK_LOADFUNCTION(vkEnumerateInstanceLayerProperties);
		VK_LOADFUNCTION(vkEnumeratePhysicalDevices);
		VK_LOADFUNCTION(vkGetInstanceProcAddr);
		VK_LOADFUNCTION(vkGetPhysicalDeviceFeatures);
		VK_LOADFUNCTION(vkGetPhysicalDeviceProperties);
		VK_LOADFUNCTION(vkGetPhysicalDeviceQueueFamilyProperties);
		VK_LOADFUNCTION(vkCreateDevice);
		VK_LOADFUNCTION(vkDestroyDevice);
		VK_LOADFUNCTION(vkCreateSwapchainKHR);
		VK_LOADFUNCTION(vkDestroySwapchainKHR);
		VK_LOADFUNCTION(vkGetPhysicalDeviceSurfaceSupportKHR);
		VK_LOADFUNCTION(vkEnumerateDeviceExtensionProperties);
		VK_LOADFUNCTION(vkDestroySurfaceKHR);
		VK_LOADFUNCTION(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
		VK_LOADFUNCTION(vkGetPhysicalDeviceSurfaceFormatsKHR);
		VK_LOADFUNCTION(vkGetPhysicalDeviceSurfacePresentModesKHR);

		return true;
	}
}

#endif



/////////////////////////////////////////////////////////////
VkResult VKAPI_PTR vkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkDebugReportCallbackEXT * pCallback)
{
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	return func(instance, pCreateInfo, pAllocator, pCallback);
}



/////////////////////////////////////////////////////////////
void VKAPI_PTR vkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks * pAllocator)
{
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	func(instance, callback, pAllocator);
}
