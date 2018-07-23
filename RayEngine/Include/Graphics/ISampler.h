#pragma once

#include "..\Defines.h"
#include "..\Types.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		struct SamplerInfo
		{

		};



		/////////////////////////////////////////////////////////////
		class ISampler
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