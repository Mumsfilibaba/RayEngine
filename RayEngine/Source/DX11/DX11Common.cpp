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

#include "RayEngine.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11/DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GetHighestSupportingSamples(ID3D11Device* pD3D11Device, uint32* count, uint32* quality, uint32 requested, DXGI_FORMAT format)
		{
			if (requested < 2)
			{
				*count = 1;
				*quality = 0;
				return;
 			}

			UINT tempQuality = 0;
			uint32 samples = requested;

			while (samples > 0 && tempQuality == 0)
			{
				pD3D11Device->CheckMultisampleQualityLevels(format, samples, &tempQuality);
				if (tempQuality < 1)
					samples /= 2;
			}

			*count = samples;
			*quality = (tempQuality - 1);
		}
	}
}

#endif