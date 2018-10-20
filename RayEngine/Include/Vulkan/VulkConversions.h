#pragma once

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline VkFormat ReToVkFormat(FORMAT format)
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


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline VkPrimitiveTopology ReToVkPrimitiveTopology(PRIMITIVE_TOPOLOGY topology)
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


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline VkBlendOp ReToVkBlendOp(BLEND_OPERATION operation)
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


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline VkBlendFactor ReToVkBlendFactor(BLEND_TYPE type)
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


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline VkSampleCountFlagBits ReToVkSampleCount(MSAA_SAMPLE_COUNT sampleCount)
		{
			switch (sampleCount)
			{
			case MSAA_SAMPLE_COUNT_2: return VK_SAMPLE_COUNT_2_BIT;
			case MSAA_SAMPLE_COUNT_4: return VK_SAMPLE_COUNT_4_BIT;
			case MSAA_SAMPLE_COUNT_8: return VK_SAMPLE_COUNT_8_BIT;
			case MSAA_SAMPLE_COUNT_16: return VK_SAMPLE_COUNT_16_BIT;
			case MSAA_SAMPLE_COUNT_1:
			default: return VK_SAMPLE_COUNT_1_BIT;
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline VkCompareOp ReToVkCompareOp(COMPARISON_FUNC comparison)
		{
			switch (comparison)
			{
			case COMPARISON_FUNC_NEVER: return VK_COMPARE_OP_NEVER;
			case COMPARISON_FUNC_LESS: return VK_COMPARE_OP_LESS;
			case COMPARISON_FUNC_EQUAL: return VK_COMPARE_OP_EQUAL;
			case COMPARISON_FUNC_LESS_EQUAL: return VK_COMPARE_OP_LESS_OR_EQUAL;
			case COMPARISON_FUNC_GREATER: return VK_COMPARE_OP_GREATER;
			case COMPARISON_FUNC_NOT_EQUAL: return VK_COMPARE_OP_NOT_EQUAL;
			case COMPARISON_FUNC_GREATER_EQUAL: return VK_COMPARE_OP_GREATER_OR_EQUAL;
			case COMPARISON_FUNC_ALWAYS:
			case COMPARISON_FUNC_UNKNOWN:
			default: return VK_COMPARE_OP_ALWAYS;
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline VkStencilOp ReToVkStencilOp(STENCIL_OPERATION operation)
		{
			switch (operation)
			{
			case STENCIL_OPERATION_ZERO: return VK_STENCIL_OP_ZERO;
			case STENCIL_OPERATION_REPLACE: return VK_STENCIL_OP_REPLACE;
			case STENCIL_OPERATION_INVERT: return VK_STENCIL_OP_INVERT;
			case STENCIL_OPERATION_INCR: return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
			case STENCIL_OPERATION_DECR: return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
			case STENCIL_OPERATION_KEEP:
			case STENCIL_OPERATION_UNKNOWN:
			default: return VK_STENCIL_OP_KEEP;
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline VkStencilOpState ReToVkStencilOpState(const StencilOperationInfo& info)
		{
			VkStencilOpState state = {};
			state.failOp = ReToVkStencilOp(info.StencilFailOperation);
			state.passOp = ReToVkStencilOp(info.StencilPassOperation);
			state.depthFailOp = ReToVkStencilOp(info.StencilDepthFailOperation);
			state.compareOp = ReToVkCompareOp(info.StencilFunc);

			//TODO: Fix this
			state.compareMask = 0xFFFFFFFF;
			state.writeMask = state.compareMask;

			state.reference = 0;

			return state;
		}
	}
}