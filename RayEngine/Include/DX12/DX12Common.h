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
#include <d3d12.h>
#include <dxgi1_6.h>



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace RayEngine
{
	namespace Graphics
	{
		RESOURCE_STATE DXToREResourceState(D3D12_RESOURCE_STATES state);
		D3D12_RESOURCE_STATES ReToDXResourceState(RESOURCE_STATE state);
		void D3D12SetName(ID3D12Object* pObject, const std::string& name);
		D3D12_BLEND ReToDX12Blend(BLEND_TYPE type);
		D3D12_BLEND_OP ReToDX12BlendOp(BLEND_OPERATION operation);
		D3D12_COMPARISON_FUNC ReToDX12ComparisonFunc(COMPARISON_FUNC func);
		D3D12_STENCIL_OP ReToDX12StencilOp(STENCIL_OPERATION operation);
		D3D12_DEPTH_STENCILOP_DESC ReToDX12StencilOpDesc(const StencilOperationInfo& info);
		D3D12_FILTER ReToDX12Filter(SAMPLER_FILTER_MODE mode);
		D3D12_TEXTURE_ADDRESS_MODE ReToDX12TextureAdressMode(SAMPLER_ADRESS_MODE mode);
		D3D12_SHADER_VISIBILITY ReShaderTypeToDX12ShaderVisibility(SHADER_TYPE type);
	}
}

#endif