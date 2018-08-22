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
#include "..\Defines.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\Types.h"
#include <d3dcommon.h>
#include <wrl\client.h>
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