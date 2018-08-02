#include <sstream>
#include "..\..\Include\DX12\DX12Common.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		RESOURCE_STATE DXToREResourceState(D3D12_RESOURCE_STATES state)
		{
			switch (state)
			{
			case D3D12_RESOURCE_STATE_DEPTH_WRITE: return RESOURCE_STATE_DEPTH_WRITE;
			case D3D12_RESOURCE_STATE_RENDER_TARGET: return RESOURCE_STATE_RENDER_TARGET;
			case D3D12_RESOURCE_STATE_COMMON:
			default: return RESOURCE_STATE_COMMON;
			}
		}



		/////////////////////////////////////////////////////////////
		D3D12_RESOURCE_STATES ReToDXResourceState(RESOURCE_STATE state)
		{
			switch (state)
			{
			case RESOURCE_STATE_DEPTH_WRITE: return D3D12_RESOURCE_STATE_DEPTH_WRITE;
			case RESOURCE_STATE_RENDER_TARGET: return D3D12_RESOURCE_STATE_RENDER_TARGET;
			case RESOURCE_STATE_PRESENT:
			case RESOURCE_STATE_COMMON:
			default: return D3D12_RESOURCE_STATE_COMMON;
			}
		}



		/////////////////////////////////////////////////////////////
		void D3D12SetName(ID3D12Object* pObject, const std::string& name)
		{
			std::wstringstream wName;
			wName << name.c_str();

			if FAILED(pObject->SetName(wName.str().c_str()))
				return;
		}



		/////////////////////////////////////////////////////////////
		D3D12_BLEND ReToDX12Blend(BLEND_TYPE type)
		{
			switch (type)
			{
			case BLEND_TYPE_ONE: return D3D12_BLEND_ONE;
			case BLEND_TYPE_SRC_COLOR: return D3D12_BLEND_SRC_COLOR;
			case BLEND_TYPE_INV_SRC_COLOR: return D3D12_BLEND_INV_SRC_COLOR;
			case BLEND_TYPE_SRC_ALPHA: return D3D12_BLEND_SRC_ALPHA;
			case BLEND_TYPE_INV_SRC_ALPHA: return D3D12_BLEND_INV_SRC_ALPHA;
			case BLEND_TYPE_DEST_ALPHA: return D3D12_BLEND_DEST_ALPHA;
			case BLEND_TYPE_INV_DEST_ALPHA: return D3D12_BLEND_INV_DEST_ALPHA;
			case BLEND_TYPE_DEST_COLOR: return D3D12_BLEND_DEST_COLOR;
			case BLEND_TYPE_INV_DEST_COLOR: return D3D12_BLEND_INV_DEST_COLOR;
			case BLEND_TYPE_SRC_ALPHA_SAT: return D3D12_BLEND_SRC_ALPHA_SAT;
			case BLEND_TYPE_BLEND_FACTOR: return D3D12_BLEND_BLEND_FACTOR;
			case BLEND_TYPE_INV_BLEND_FACTOR: return D3D12_BLEND_INV_BLEND_FACTOR;
			case BLEND_TYPE_ZERO:
			case BLEND_TYPE_UNKNOWN:
			default: return D3D12_BLEND_ZERO;
			}
		}



		/////////////////////////////////////////////////////////////
		D3D12_BLEND_OP ReToDX12BlendOp(BLEND_OPERATION operation)
		{
			switch (operation)
			{
			case BLEND_OPERATION_SUBTRACT: return D3D12_BLEND_OP_SUBTRACT;
			case BLEND_OPERATION_REV_SUBTRACT: return D3D12_BLEND_OP_REV_SUBTRACT;
			case BLEND_OPERATION_MIN: return D3D12_BLEND_OP_MIN;
			case BLEND_OPERATION_MAX: return D3D12_BLEND_OP_MAX;
			case BLEND_OPERATION_ADD:
			case BLEND_OPERATION_UNKNOWN:
			default: return D3D12_BLEND_OP_ADD;
			}
		}



		/////////////////////////////////////////////////////////////
		D3D12_COMPARISON_FUNC ReToDX12ComparisonFunc(COMPARISON_FUNC func)
		{
			switch (func)
			{
			case COMPARISON_FUNC_NEVER: return D3D12_COMPARISON_FUNC_NEVER;
			case COMPARISON_FUNC_LESS: return D3D12_COMPARISON_FUNC_LESS;
			case COMPARISON_FUNC_EQUAL: return D3D12_COMPARISON_FUNC_EQUAL;
			case COMPARISON_FUNC_LESS_EQUAL: return D3D12_COMPARISON_FUNC_LESS_EQUAL;
			case COMPARISON_FUNC_GREATER: return D3D12_COMPARISON_FUNC_GREATER;
			case COMPARISON_FUNC_NOT_EQUAL: return D3D12_COMPARISON_FUNC_NOT_EQUAL;
			case COMPARISON_FUNC_GREATER_EQUAL: return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
			case COMPARISON_FUNC_ALWAYS:
			case COMPARISON_FUNC_UNKNOWN:
			default: return D3D12_COMPARISON_FUNC_ALWAYS;
			}
		}



		/////////////////////////////////////////////////////////////
		D3D12_STENCIL_OP ReToDX12StencilOp(STENCIL_OPERATION operation)
		{
			switch (operation)
			{
			case STENCIL_OPERATION_ZERO: return D3D12_STENCIL_OP_ZERO;
			case STENCIL_OPERATION_REPLACE: return D3D12_STENCIL_OP_REPLACE;
			case STENCIL_OPERATION_INCR_SAT: return D3D12_STENCIL_OP_INCR_SAT;
			case STENCIL_OPERATION_DECR_SAT: return D3D12_STENCIL_OP_DECR_SAT;
			case STENCIL_OPERATION_INVERT: return D3D12_STENCIL_OP_INVERT;
			case STENCIL_OPERATION_INCR: return D3D12_STENCIL_OP_INCR;
			case STENCIL_OPERATION_DECR: return D3D12_STENCIL_OP_DECR;
			case STENCIL_OPERATION_KEEP:
			case STENCIL_OPERATION_UNKNOWN:
			default: return D3D12_STENCIL_OP_KEEP;
			}
		}



		/////////////////////////////////////////////////////////////
		D3D12_DEPTH_STENCILOP_DESC ReToDX12StencilOpDesc(const StencilOperationInfo& info)
		{
			D3D12_DEPTH_STENCILOP_DESC desc = {};
			desc.StencilFunc = ReToDX12ComparisonFunc(info.StencilFunc);
			desc.StencilFailOp = ReToDX12StencilOp(info.StencilFailOperation);
			desc.StencilPassOp = ReToDX12StencilOp(info.StencilPassoperation);
			desc.StencilDepthFailOp = ReToDX12StencilOp(info.StencilDepthFailOperation);

			return desc;
		}
	}
}

#endif