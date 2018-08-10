#pragma once

#include "..\DXBase\DXCommon.h"

#if defined(RE_PLATFORM_WINDOWS)
#include <d3d11.h>
#include <wrl\client.h>

namespace RayEngine
{
	namespace Graphics
	{
		D3D11_BLEND ReToDX11Blend(BLEND_TYPE type);
		D3D11_BLEND_OP ReToDX11BlendOp(BLEND_OPERATION operation);
		D3D11_COMPARISON_FUNC ReToDX11ComparisonFunc(COMPARISON_FUNC func);
		D3D11_STENCIL_OP ReToDX11StencilOp(STENCIL_OPERATION operation);
		D3D11_DEPTH_STENCILOP_DESC ReToDX11StencilOpDesc(const StencilOperationInfo& info);
		D3D11_TEXTURE_ADDRESS_MODE ReToDX11TextureAdressMode(SAMPLER_ADRESS_MODE mode);
		D3D11_FILTER ReToDX11Filter(SAMPLER_FILTER_MODE mode);
	}
}
#endif