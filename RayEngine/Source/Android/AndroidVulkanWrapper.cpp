#include "..\..\Include\Android\AndroidVulkanWrapper.h"

#if defined(RE_PLATFORM_ANDROID)
#include <dlfcn.h>

#define VK_LOADFUNCTION(function) (function = reinterpret_cast<PFN_##function>(dlsym(libVulkan, #function)))

/////////////////////////////////////////////////////////////
PFN_vkCreateInstance vkCreateInstance = nullptr;
PFN_vkDestroyInstance vkDestroyInstance = nullptr;

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

		return true;
	}
}

#endif