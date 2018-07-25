#pragma once

#include "..\IReferenceCounter.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		struct SamplerInfo
		{
			std::string Name = "";
		};



		/////////////////////////////////////////////////////////////
		class ISampler : public IReferenceCounter
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