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
		VkResult VulkanCreateSwapchainSurface(VkInstance instance, VkSurfaceKHR* pSurface, const System::NativeWindowHandle& pWindow);
		VkFormat ReToVkFormat(FORMAT format);
		VkPrimitiveTopology ReToVkPrimitiveTopology(PRIMITIVE_TOPOLOGY topology);
		VkBlendOp ReToVkBlendOp(BLEND_OPERATION operation);
		VkBlendFactor ReToVkBlendFactor(BLEND_TYPE type);
		VkSampleCountFlagBits ReToVkSampleCount(MSAA_SAMPLE_COUNT sampleCount);
		VkCompareOp ReToVkCompareOp(COMPARISON_FUNC comparison);
		VkStencilOp ReToVkStencilOp(STENCIL_OPERATION operation);
		VkStencilOpState ReToVkStencilOpState(const StencilOperationInfo& info);
	}
}