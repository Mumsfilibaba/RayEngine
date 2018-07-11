#pragma once

#include "ICommandQueue.h"
#include "IFence.h"
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

			//Create a queue for graphics- and computecommands
			virtual bool CreateCommandQueue(ICommandQueue** commandQueue, const CommanQueueInfo& info) const = 0;
			//Create a fence for synchronization between CPU and GPU
			virtual bool CreateFence(IFence** fence) const = 0;
		};
	}
}