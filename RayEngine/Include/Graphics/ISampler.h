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
#include <RayEngine.h>
#include <Interfaces/IObject.h>

#if defined(max)
#undef max
#endif

namespace RayEngine
{
	namespace Graphics
	{
		/*////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////*/
		struct RAYENGINE_API SamplerDesc
		{
			SAMPLER_FILTER_MODE FilterMode = SAMPLER_FILTER_MODE_UNKNOWN;
			SAMPLER_ADRESS_MODE AdressU = SAMPLER_ADRESS_MODE_UNKNOWN;
			SAMPLER_ADRESS_MODE AdressV = SAMPLER_ADRESS_MODE_UNKNOWN;
			SAMPLER_ADRESS_MODE AdressW = SAMPLER_ADRESS_MODE_UNKNOWN;
			float MipLODBias;
			float MinLOD;
			float MaxLOD = std::numeric_limits<float>::max();
			uint32 MaxAnistropy;
			COMPARISON_FUNC ComparisonFunc;
			struct
			{
				float R;
				float G;
				float B;
				float A;
			} BorderColor;
		};


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class RAYENGINE_API ISampler : public IObject
		{
			RE_INTERFACE(ISampler);
		
		public:
			ISampler() {}
			~ISampler() {}

			/*////////////////////////////////////////////////////////////

				Retrives the descriptor that was used to create the
				object.

				pDesc - A valid pointer to a Desc

			////////////////////////////////////////////////////////////*/
			virtual void GetDesc(SamplerDesc* pDesc) const  = 0;
		};
	}
}