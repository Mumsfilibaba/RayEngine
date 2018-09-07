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
#include "IDeviceObject.h"

namespace RayEngine
{
	namespace Graphics
	{
		class IResource : public IDeviceObject
		{
		public:
			IResource(IResource&& other) = delete;
			IResource(const IResource& other) = delete;
			IResource& operator=(IResource&& other) = delete;
			IResource& operator=(const IResource& other) = delete;

		public:
			IResource() {}
			~IResource() {}
		};
	}
}