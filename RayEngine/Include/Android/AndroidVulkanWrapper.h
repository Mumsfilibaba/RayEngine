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

#endif