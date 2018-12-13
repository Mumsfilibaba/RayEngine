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
#include "DXBase\DXCommon.h"

#if defined(RE_PLATFORM_WINDOWS)
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


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline void GetHighestSupportingSamples(ID3D12Device* pD3D12Device, uint32* count, uint32* quality, uint32 requested, DXGI_FORMAT format)
		{
			if (requested < 2)
			{
				*count = 1;
				*quality = 0;
				return;
			}

			D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msaaQuality = {};
			msaaQuality.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
			msaaQuality.Format = format;
			msaaQuality.SampleCount = requested;

			HRESULT hr = 0;
			while (msaaQuality.SampleCount > 1)
			{
				hr = pD3D12Device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msaaQuality, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS));
				if (SUCCEEDED(hr))
					break;

				msaaQuality.SampleCount /= 2;
			}

			if (msaaQuality.NumQualityLevels == 0)
			{
				*count = 1;
				*quality = 0;
			}
			else
			{
				*count = msaaQuality.SampleCount;
				*quality = msaaQuality.NumQualityLevels - 1;
			}
		}
	}
}

#endif