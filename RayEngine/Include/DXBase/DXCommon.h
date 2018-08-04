#pragma once

#include "..\Defines.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\Types.h"

#include <d3dcommon.h>
#include <dxgi.h>

#if !defined(D3DRelease)
#define D3DRelease(IObject) IObject->Release(); IObject = nullptr
#endif

#if !defined(D3DRelease_S)
#define D3DRelease_S(IObject) if (IObject != nullptr) { D3DRelease(IObject); }
#endif


namespace RayEngine
{
	std::string DXErrorString(HRESULT hr);
	DXGI_FORMAT ReToDXFormat(FORMAT format);
	D3D_PRIMITIVE_TOPOLOGY ReToDXTopology(PRIMITIVE_TOPOLOGY topology);
}
#endif