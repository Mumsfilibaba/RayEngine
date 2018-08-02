#include "..\..\Include\Vulkan\VulkCommon.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\Win32\Win32WindowImpl.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		VkResult VulkanCreateSwapchainSurface(VkInstance instance, VkSurfaceKHR* surface, const System::IWindowImpl* window)
		{
			using namespace RayEngine;
			using namespace System;

			const Win32WindowImpl* w = reinterpret_cast<const Win32WindowImpl*>(window);

			VkWin32SurfaceCreateInfoKHR info = {};
			info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			info.flags = 0;
			info.pNext = nullptr;
			info.hwnd = w->GetHWND();
			info.hinstance = w->GetHINSTANCE();

			auto vkCreateWin32SurfaceKHR = reinterpret_cast<PFN_vkCreateWin32SurfaceKHR>(vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR"));
			VkResult result = vkCreateWin32SurfaceKHR(instance, &info, nullptr, surface);

			return result;
		}
	}
}

#endif