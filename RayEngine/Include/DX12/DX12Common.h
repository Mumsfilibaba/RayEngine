#pragma once
#include "..\DXBase\DXCommon.h"

#if defined(RE_PLATFORM_WINDOWS)
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl\client.h>

#define D3DRelease(IObject) IObject->Release(); IObject = nullptr
#define D3DRelease_S(IObject) if (IObject != nullptr) { D3DRelease(IObject); }



/////////////////////////////////////////////////////////////
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
	}
}

#endif