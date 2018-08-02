#pragma once

#include "..\Defines.h"
#include "..\System\Window.h"

#if defined(RE_PLATFORM_ANDROID)
#include "..\Android\AndroidVulkanWrapper.h"
#elif defined(RE_PLATFORM_WINDOWS)
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#endif


namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		VkResult VulkanCreateSwapchainSurface(VkInstance instance, VkSurfaceKHR* pSurface, const System::IWindowImpl* pWindow);
		VkFormat ReToVkFormat(FORMAT format);
	}
}