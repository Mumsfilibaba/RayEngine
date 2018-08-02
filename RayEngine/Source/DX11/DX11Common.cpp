#include "..\..\Include\DX11\DX11Common.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		D3D11_BLEND ReToDX11Blend(BLEND_TYPE type)
		{
			switch (type)
			{
			case BLEND_TYPE_ONE: return D3D11_BLEND_ONE;
			case BLEND_TYPE_SRC_COLOR: return D3D11_BLEND_SRC_COLOR;
			case BLEND_TYPE_INV_SRC_COLOR: return D3D11_BLEND_INV_SRC_COLOR;
			case BLEND_TYPE_SRC_ALPHA: return D3D11_BLEND_SRC_ALPHA;
			case BLEND_TYPE_INV_SRC_ALPHA: return D3D11_BLEND_INV_SRC_ALPHA;
			case BLEND_TYPE_DEST_ALPHA: return D3D11_BLEND_DEST_ALPHA;
			case BLEND_TYPE_INV_DEST_ALPHA: return D3D11_BLEND_INV_DEST_ALPHA;
			case BLEND_TYPE_DEST_COLOR: return D3D11_BLEND_DEST_COLOR;
			case BLEND_TYPE_INV_DEST_COLOR: return D3D11_BLEND_INV_DEST_COLOR;
			case BLEND_TYPE_SRC_ALPHA_SAT: return D3D11_BLEND_SRC_ALPHA_SAT;
			case BLEND_TYPE_BLEND_FACTOR: return D3D11_BLEND_BLEND_FACTOR;
			case BLEND_TYPE_INV_BLEND_FACTOR: return D3D11_BLEND_INV_BLEND_FACTOR;
			case BLEND_TYPE_ZERO:
			case BLEND_TYPE_UNKNOWN:
			default: return D3D11_BLEND_ZERO;
			}
		}



		/////////////////////////////////////////////////////////////
		D3D11_BLEND_OP ReToDX11BlendOp(BLEND_OPERATION operation) 
		{
			switch (operation)
			{
			case BLEND_OPERATION_SUBTRACT: return D3D11_BLEND_OP_SUBTRACT;
			case BLEND_OPERATION_REV_SUBTRACT: return D3D11_BLEND_OP_REV_SUBTRACT;
			case BLEND_OPERATION_MIN: return D3D11_BLEND_OP_MIN;
			case BLEND_OPERATION_MAX: return D3D11_BLEND_OP_MAX;
			case BLEND_OPERATION_ADD: 
			case BLEND_OPERATION_UNKNOWN:
			default: return D3D11_BLEND_OP_ADD;
			}
		}



		/////////////////////////////////////////////////////////////
		D3D11_COMPARISON_FUNC ReToDX11ComparisonFunc(COMPARISON_FUNC func)
		{
			switch (func)
			{
			case COMPARISON_FUNC_NEVER: return D3D11_COMPARISON_NEVER;
			case COMPARISON_FUNC_LESS: return D3D11_COMPARISON_LESS;
			case COMPARISON_FUNC_EQUAL: return D3D11_COMPARISON_EQUAL;
			case COMPARISON_FUNC_LESS_EQUAL: return D3D11_COMPARISON_LESS_EQUAL;
			case COMPARISON_FUNC_GREATER: return D3D11_COMPARISON_GREATER;
			case COMPARISON_FUNC_NOT_EQUAL: return D3D11_COMPARISON_NOT_EQUAL;
			case COMPARISON_FUNC_GREATER_EQUAL: return D3D11_COMPARISON_GREATER_EQUAL;
			case COMPARISON_FUNC_ALWAYS:
			case COMPARISON_FUNC_UNKNOWN:
			default: return D3D11_COMPARISON_ALWAYS;
			}
		}



		/////////////////////////////////////////////////////////////
		D3D11_STENCIL_OP ReToDX11StencilOp(STENCIL_OPERATION operation)
		{
			switch (operation)
			{
			case STENCIL_OPERATION_ZERO: return D3D11_STENCIL_OP_ZERO;
			case STENCIL_OPERATION_REPLACE: return D3D11_STENCIL_OP_REPLACE;
			case STENCIL_OPERATION_INCR_SAT: return D3D11_STENCIL_OP_INCR_SAT;
			case STENCIL_OPERATION_DECR_SAT: return D3D11_STENCIL_OP_DECR_SAT;
			case STENCIL_OPERATION_INVERT: return D3D11_STENCIL_OP_INVERT;
			case STENCIL_OPERATION_INCR: return D3D11_STENCIL_OP_INCR;
			case STENCIL_OPERATION_DECR: return D3D11_STENCIL_OP_DECR;
			case STENCIL_OPERATION_KEEP:
			case STENCIL_OPERATION_UNKNOWN:
			default: return D3D11_STENCIL_OP_KEEP;
			}
		}



		/////////////////////////////////////////////////////////////
		D3D11_DEPTH_STENCILOP_DESC ReToDX11StencilOpDesc(const StencilOperationInfo& info)
		{
			D3D11_DEPTH_STENCILOP_DESC desc = {};
			desc.StencilFunc = ReToDX11ComparisonFunc(info.StencilFunc);
			desc.StencilPassOp = ReToDX11StencilOp(info.StencilPassoperation);
			desc.StencilFailOp = ReToDX11StencilOp(info.StencilFailOperation);
			desc.StencilDepthFailOp = ReToDX11StencilOp(info.StencilDepthFailOperation);

			return desc;
		}
	}
}

#endif