#pragma once

#include "..\IReferenceCounter.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class IDevice;



		/////////////////////////////////////////////////////////////
		class IDeviceObject : public IReferenceCounter
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