#pragma once

#include "..\RefCounter.h"

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
		};



		/////////////////////////////////////////////////////////////
		class ISampler : public RefCounter
		{
		public:
			ISampler(ISampler&& other) = delete;
			ISampler(const ISampler& other) = delete;
			ISampler& operator=(ISampler&& other) = delete;
			ISampler& operator=(const ISampler& other) = delete;

		public:
			ISampler() {}
			virtual ~ISampler() {}

			virtual IDevice* GetDevice();
		};
	}
}