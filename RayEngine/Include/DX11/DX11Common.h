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
		void GetHighestSupportingSamples(ID3D11Device* pD3D11Device, uint32* count, uint32* quality, uint32 requested, DXGI_FORMAT format);
	}
}
#endif