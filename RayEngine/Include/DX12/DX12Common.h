#pragma once

#include "..\Defines.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\Types.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl\client.h>

#define D3DRelease(IObject) IObject->Release(); IObject = nullptr
#define D3DRelease_S(IObject) if (IObject != nullptr) { D3DRelease(IObject); }



/////////////////////////////////////////////////////////////
namespace RayEngine
{
	DXGI_FORMAT ReToDXFormat(FORMAT format);
	RESOURCE_STATE DXToREResourceState(D3D12_RESOURCE_STATES state);
	D3D12_RESOURCE_STATES ReToDXResourceState(RESOURCE_STATE state);
	void D3D12SetName(ID3D12Object* pObject, const std::string& name);
}

#endif