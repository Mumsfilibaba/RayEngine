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

#include "..\..\Include\Graphics\IFactory.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\Vulkan\VulkFactory.h"
#include "..\..\Include\DX12\DX12Factory.h"
#include "..\..\Include\DX11\DX11Factory.h"

namespace RayEngine
{
	namespace Graphics
	{	
		/////////////////////////////////////////////////////////////
		IFactory* IFactory::Create(GRAPHICS_API api, bool debugLayers)
		{
			IFactory* pFactory = nullptr;

			if (api == GRAPHICS_API_D3D12)
				pFactory = new DX12Factory(debugLayers);
			else if (api == GRAPHICS_API_D3D11)
				pFactory = new DX11Factory(debugLayers);
			else if (api == GRAPHICS_API_VULKAN)
				pFactory = new VulkFactory(debugLayers);

			return pFactory;
		}
	}
}

#endif