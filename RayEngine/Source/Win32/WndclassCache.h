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

#include "../../Include/RayEngine.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	class WndclassCache
	{
		RE_STATIC_CLASS(WndclassCache);

	private:
		struct Cache
		{
			WNDCLASSEX data = {};
			int32 count = 0;
		};

	public:
		static bool Register(WNDCLASSEX& wc);
		static void Unregister(const std::string& name, HINSTANCE hInstance);
		static bool GetClass(const std::string& name, WNDCLASSEX& wc);

	private:
		static std::unordered_map<std::string, Cache> s_Classes;
	};
}

#endif