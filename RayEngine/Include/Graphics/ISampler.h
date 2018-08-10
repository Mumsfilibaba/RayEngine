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
		/////////////////////////////////////////////////////////////
		class IDevice;



		/////////////////////////////////////////////////////////////
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



		/////////////////////////////////////////////////////////////
		class ISampler : public IDeviceObject
		{
		public:
			ISampler(ISampler&& other) = delete;
			ISampler(const ISampler& other) = delete;
			ISampler& operator=(ISampler&& other) = delete;
			ISampler& operator=(const ISampler& other) = delete;
		
		public:
			ISampler() {}
			virtual ~ISampler() {}
		};
	}
}