#pragma once

#include "..\Defines.h"

#if defined(RE_PLATFORM_ANDROID)
#define VK_USE_PLATFORM_ANDROID_KHR
#include "..\Types.h"

#define VK_NO_PROTOTYPES 1
#include <vulkan\vulkan.h>

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	extern bool InitializeVulkan();
}

/////////////////////////////////////////////////////////////
extern PFN_vkCreateInstance vkCreateInstance;
extern PFN_vkDestroyInstance vkDestroyInstance;
extern PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
extern PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
extern PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
extern PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
extern PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
extern PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
extern PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
extern PFN_vkCreateDevice vkCreateDevice;
extern PFN_vkDestroyDevice vkDestroyDevice;
extern PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
extern PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
extern PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
extern PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
extern PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
extern PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
extern PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
extern PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;



/////////////////////////////////////////////////////////////
VkResult VKAPI_PTR vkCreateDebugReportCallbackEXT(VkInstance instance,
	const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator,
	VkDebugReportCallbackEXT* pCallback);

void VKAPI_PTR vkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback,
	const VkAllocationCallbacks* pAllocator);

#endif