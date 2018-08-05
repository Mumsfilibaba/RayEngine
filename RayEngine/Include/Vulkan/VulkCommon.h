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
		VkPrimitiveTopology ReToVkPrimitiveTopology(PRIMITIVE_TOPOLOGY topology);
		VkBlendOp ReToVkBlendOp(BLEND_OPERATION operation);
		VkBlendFactor ReToVkBlendFactor(BLEND_TYPE type);
		VkSampleCountFlagBits ReToVkSampleCount(MSAA_SAMPLE_COUNT sampleCount);
		VkCompareOp ReToVkCompareOp(COMPARISON_FUNC comparison);
		VkStencilOp ReToVkStencilOp(STENCIL_OPERATION operation);
		VkStencilOpState ReToVkStencilOpState(const StencilOperationInfo& info);
	}
}