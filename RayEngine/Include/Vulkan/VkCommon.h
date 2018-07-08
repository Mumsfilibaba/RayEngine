#pragma once

#include "..\Defines.h"
#include "..\System\Window.h"

#if defined(RE_PLATFORM_ANDROID)
#include "..\Android\AndroidVulkanWrapper.h"
#elif defined(RE_PLATFORM_WINDOWS)
#endif

extern VkResult VulkanCreateSwapchainSurface(VkInstance instance, VkSurfaceKHR* surface, const RayEngine::System::IWindowImpl* window);