#pragma once

#include "..\Defines.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\Types.h"
#include <dxgi.h>

#define D3DRelease(IObject) IObject->Release(); IObject = nullptr
#define D3DRelease_S(IObject) if (IObject != nullptr) { D3DRelease(IObject); }



namespace RayEngine
{
	std::string DXErrorString(HRESULT hr);
	DXGI_FORMAT ReToDXFormat(FORMAT format);
}
#endif