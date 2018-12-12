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

#include "RayEngine.h"
#include "GlImpl.h"
#include "GLConversions.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void QueryExtensionsFromString(std::vector<std::string>& extensions, const std::string& str);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline GLNativeContext GetCurrentContext()
		{
#if defined(RE_PLATFORM_WINDOWS)
			return wglGetCurrentContext();
#elif defined(RE_PLATFORM_LINUX)
			return glXGetCurrentContext();
#endif
		}
	}
}