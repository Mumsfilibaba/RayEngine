#pragma once

#include "..\ReferenceCounter.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class IDevice;



		/////////////////////////////////////////////////////////////
		class IDeviceObject : public ReferenceCounter
		{
		public:
			IDeviceObject(IDeviceObject&& other) = delete;
			IDeviceObject(const IDeviceObject& other) = delete;
			IDeviceObject& operator=(IDeviceObject&& other) = delete;
			IDeviceObject& operator=(const IDeviceObject& other) = delete;
			
		public:
			IDeviceObject() {}
			virtual ~IDeviceObject() {}

			virtual void QueryDevice(IDevice** ppDevice) const = 0;
		};
	}
}