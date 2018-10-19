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
#include <limits>
#include <string>
#include "IDeviceObject.h"

#if defined(max)
#undef max
#endif

namespace RayEngine
{
	namespace Graphics
	{
		/*////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////*/
		struct SamplerInfo
		{
			std::string Name = "";
			SAMPLER_FILTER_MODE FilterMode = SAMPLER_FILTER_MODE_UNKNOWN;
			SAMPLER_ADRESS_MODE AdressU = SAMPLER_ADRESS_MODE_UNKNOWN;
			SAMPLER_ADRESS_MODE AdressV = SAMPLER_ADRESS_MODE_UNKNOWN;
			SAMPLER_ADRESS_MODE AdressW = SAMPLER_ADRESS_MODE_UNKNOWN;
			float MipLODBias = 0.0f;
			float MinLOD = 0.0f;
			float MaxLOD = std::numeric_limits<float>::max();
			uint32 MaxAnistropy = 16;
			COMPARISON_FUNC ComparisonFunc = COMPARISON_FUNC_UNKNOWN;
			struct
			{
				float R = 0.0f;
				float G = 0.0f;
				float B = 0.0f;
				float A = 0.0f;
			} BorderColor;
		};



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class ISampler : public IDeviceObject
		{
			RE_INTERFACE(ISampler);
		
		public:
			ISampler() {}
			~ISampler() {}
		};
	}
}