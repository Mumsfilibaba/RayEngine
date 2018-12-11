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
#include "Interfaces/IObject.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class IDevice;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class RAYENGINE_API IDeviceObject : public IObject
		{
			RE_INTERFACE(IDeviceObject);
			
		public:
			IDeviceObject() {}
			~IDeviceObject() {}

			/*////////////////////////////////////////////////////////////

				Sets the name of the object.

				name - A new name of the object.

			////////////////////////////////////////////////////////////*/
			virtual void SetName(const std::string& name) = 0;

			/*////////////////////////////////////////////////////////////
				
				Querys the device that created the object.

				ppDevice - A valid pointer to a ppDevice - interface.

				Release needs to be called on the returned object when
				it is no longer needed.

			////////////////////////////////////////////////////////////*/
			virtual void QueryDevice(IDevice** ppDevice) const = 0;
		};
	}
}