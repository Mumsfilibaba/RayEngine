#pragma once

#include "AdapterInfo.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		struct DeviceInfo
		{
			AdapterInfo* Adapter = nullptr;
		};



		/////////////////////////////////////////////////////////////
		class IDevice
		{
		public:
			IDevice(IDevice&& other) = delete;
			IDevice(const IDevice& other) = delete;
			IDevice& operator=(IDevice&& other) = delete;
			IDevice& operator=(const IDevice& other) = delete;

		public:
			IDevice() {}
			virtual ~IDevice() {}
		};
	}
}