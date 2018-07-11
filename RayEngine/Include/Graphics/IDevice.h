#pragma once

#include "ICommandQueue.h"
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

			virtual bool CreateCommandQueue(ICommandQueue** commandQueue, const CommanQueueInfo& info) const = 0;
		};
	}
}