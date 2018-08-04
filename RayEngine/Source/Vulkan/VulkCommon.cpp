#include "..\..\Include\Vulkan\VulkCommon.h"

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



namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		VkFormat ReToVkFormat(FORMAT format)
		{
			switch (format)
			{
			case FORMAT_R32G32B32A32_FLOAT:		return VK_FORMAT_R32G32B32A32_SFLOAT;
			case FORMAT_R32G32B32A32_UINT:		return VK_FORMAT_R32G32B32A32_UINT;
			case FORMAT_R32G32B32A32_SINT:		return VK_FORMAT_R32G32B32A32_SINT;
			case FORMAT_R32G32B32_FLOAT:		return VK_FORMAT_R32G32B32_SFLOAT;
			case FORMAT_R32G32B32_UINT:			return VK_FORMAT_R32G32B32_UINT;
			case FORMAT_R32G32B32_SINT:			return VK_FORMAT_R32G32B32_SINT;
			case FORMAT_R16G16B16A16_FLOAT:		return VK_FORMAT_R16G16B16A16_SFLOAT;
			case FORMAT_R16G16B16A16_UNORM:		return VK_FORMAT_R16G16B16A16_UNORM;
			case FORMAT_R16G16B16A16_UINT:		return VK_FORMAT_R16G16B16A16_UINT;
			case FORMAT_R16G16B16A16_SNORM:		return VK_FORMAT_R16G16B16A16_SNORM;
			case FORMAT_R16G16B16A16_SINT:		return VK_FORMAT_R16G16B16A16_SINT;
			case FORMAT_R32G32_FLOAT:			return VK_FORMAT_R32G32_SFLOAT;
			case FORMAT_R32G32_UINT:			return VK_FORMAT_R32G32_UINT;
			case FORMAT_R32G32_SINT:			return VK_FORMAT_R32G32_SINT;
			case FORMAT_D32_FLOAT_S8X24_UINT:	return VK_FORMAT_D32_SFLOAT_S8_UINT;
			case FORMAT_R8G8B8A8_UNORM:			return VK_FORMAT_R8G8B8A8_UNORM;
			case FORMAT_R8G8B8A8_UNORM_SRGB:	return VK_FORMAT_R8G8B8A8_SRGB;
			case FORMAT_R8G8B8A8_UINT:			return VK_FORMAT_R8G8B8A8_UINT;
			case FORMAT_R8G8B8A8_SNORM:			return VK_FORMAT_R8G8B8A8_SNORM;
			case FORMAT_R8G8B8A8_SINT:			return VK_FORMAT_R8G8B8A8_SINT;
			case FORMAT_R16G16_FLOAT:			return VK_FORMAT_R16G16_SFLOAT;
			case FORMAT_R16G16_UNORM:			return VK_FORMAT_R16G16_UNORM;
			case FORMAT_R16G16_UINT:			return VK_FORMAT_R16G16_UINT;
			case FORMAT_R16G16_SNORM:			return VK_FORMAT_R16G16_SNORM;
			case FORMAT_R16G16_SINT:			return VK_FORMAT_R16G16_SINT;
			case FORMAT_D32_FLOAT:				return VK_FORMAT_D32_SFLOAT;
			case FORMAT_R32_FLOAT:				return VK_FORMAT_R32_SFLOAT;
			case FORMAT_R32_UINT:				return VK_FORMAT_R32_UINT;
			case FORMAT_R32_SINT:				return VK_FORMAT_R32_SINT;
			case FORMAT_D24_UNORM_S8_UINT:		return VK_FORMAT_D24_UNORM_S8_UINT;
			case FORMAT_R8G8_UNORM:				return VK_FORMAT_R8G8_UNORM;
			case FORMAT_R8G8_UINT:				return VK_FORMAT_R8G8_UINT;
			case FORMAT_R8G8_SNORM:				return VK_FORMAT_R8G8_SNORM;
			case FORMAT_R8G8_SINT:				return VK_FORMAT_R8G8_SINT;
			case FORMAT_R16_FLOAT:				return VK_FORMAT_R16_SFLOAT;
			case FORMAT_D16_UNORM:				return VK_FORMAT_D16_UNORM;
			case FORMAT_R16_UNORM:				return VK_FORMAT_R16_UNORM;
			case FORMAT_R16_UINT:				return VK_FORMAT_R16_UINT;
			case FORMAT_R16_SNORM:				return VK_FORMAT_R16_SNORM;
			case FORMAT_R16_SINT:				return VK_FORMAT_R16_SINT;
			case FORMAT_R8_UNORM:				return VK_FORMAT_R8_UNORM;
			case FORMAT_R8_UINT:				return VK_FORMAT_R8_UINT;
			case FORMAT_R8_SNORM:				return VK_FORMAT_R8_SNORM;
			case FORMAT_R8_SINT:				return VK_FORMAT_R8_SINT;
			case FORMAT_B8G8R8A8_UNORM:			return VK_FORMAT_B8G8R8A8_UNORM;
			case FORMAT_B8G8R8A8_UNORM_SRGB:	return VK_FORMAT_B8G8R8A8_SRGB;
			}

			return VK_FORMAT_UNDEFINED;
		}



		/////////////////////////////////////////////////////////////
		VkPrimitiveTopology ReToVkPrimitiveTopology(PRIMITIVE_TOPOLOGY topology)
		{
			switch (topology)
			{
			case PRIMITIVE_TOPOLOGY_TRIANGLELIST: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			case PRIMITIVE_TOPOLOGY_TRIANGLESTRIP: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			case PRIMITIVE_TOPOLOGY_LINELIST: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			case PRIMITIVE_TOPOLOGY_LINESTRIP: return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			case PRIMITIVE_TOPOLOGY_POINTS: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			default: return static_cast<VkPrimitiveTopology>(-1);
			}
		}



		/////////////////////////////////////////////////////////////
		VkBlendOp ReToVkBlendOp(BLEND_OPERATION operation)
		{
			switch (operation)
			{
			case BLEND_OPERATION_SUBTRACT: return VK_BLEND_OP_SUBTRACT;
			case BLEND_OPERATION_REV_SUBTRACT: return VK_BLEND_OP_REVERSE_SUBTRACT;
			case BLEND_OPERATION_MIN: return VK_BLEND_OP_MIN;
			case BLEND_OPERATION_MAX: return VK_BLEND_OP_MAX;
			case BLEND_OPERATION_ADD:
			case BLEND_OPERATION_UNKNOWN:
			default: return VK_BLEND_OP_ADD;
			}
		}



		/////////////////////////////////////////////////////////////
		VkBlendFactor ReToVkBlendFactor(BLEND_TYPE type)
		{
			switch (type)
			{
			case BLEND_TYPE_ONE: return VK_BLEND_FACTOR_ONE;
			case BLEND_TYPE_SRC_COLOR: return VK_BLEND_FACTOR_SRC_COLOR;
			case BLEND_TYPE_INV_SRC_COLOR: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
			case BLEND_TYPE_SRC_ALPHA: return VK_BLEND_FACTOR_SRC_ALPHA;
			case BLEND_TYPE_INV_SRC_ALPHA: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			case BLEND_TYPE_DEST_ALPHA: return VK_BLEND_FACTOR_DST_ALPHA;
			case BLEND_TYPE_INV_DEST_ALPHA: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
			case BLEND_TYPE_DEST_COLOR: return VK_BLEND_FACTOR_DST_COLOR;
			case BLEND_TYPE_INV_DEST_COLOR: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
			case BLEND_TYPE_SRC_ALPHA_SAT: return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
			case BLEND_TYPE_BLEND_FACTOR: return VK_BLEND_FACTOR_CONSTANT_COLOR;
			case BLEND_TYPE_INV_BLEND_FACTOR: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
			case BLEND_TYPE_ZERO:
			case BLEND_TYPE_UNKNOWN:
			default: return VK_BLEND_FACTOR_ZERO;
			}
		}
	}
}