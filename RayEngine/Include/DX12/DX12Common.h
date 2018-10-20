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

#include "DX12Conversions.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline void D3D12SetName(ID3D12Object* pObject, const std::string& name)
		{
			if (FAILED(pObject->SetName(WidenString(name).c_str())))
				return;
		}
	}
}

#endif