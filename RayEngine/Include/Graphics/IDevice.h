#pragma once

#include "AdapterInfo.h"

namespace RayEngine
{
	namespace Graphics
	{
		struct DeviceInfo
		{
			AdapterInfo* Adapter = nullptr;
		};

		class IDevice
		{
		public:
			virtual ~IDevice() {}
		};
	}
}