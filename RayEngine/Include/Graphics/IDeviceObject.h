/*////////////////////////////////////////////////////////////

Copyright 2018 Alexander Dahlin

Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in
compliance with the License. You may obtain a copy of
the License at

http ://www.apache.org/licenses/LICENSE-2.0

THIS SOFTWARE IS PROVIDED "AS IS". MEANING NO WARRANTY
OR SUPPORT IS PROVIDED OF ANY KIND.

In event of any damages, direct or indirect that can
be traced back to the use of this software, shall no
contributor be held liable. This includes computer
failure and or malfunction of any kind.

////////////////////////////////////////////////////////////*/

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