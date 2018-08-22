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

#include "..\..\Include\Defines.h"
#include "..\..\Include\Types.h"

#if defined(RE_PLATFORM_WINDOWS)

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN 1
#endif

#include <unordered_map>
#include <Windows.h>

namespace RayEngine
{
	namespace System
	{
		class WndclassCache
		{
		private:
			struct Cache
			{
				WNDCLASSEX data = {};
				int32 count = 0;
			};

		public:
			WndclassCache() = delete;
			~WndclassCache() = delete;
			WndclassCache(WndclassCache&& other) = delete;
			WndclassCache(const WndclassCache& other) = delete;
			WndclassCache& operator=(WndclassCache&& other) = delete;
			WndclassCache& operator=(const WndclassCache& other) = delete;

		public:
			static bool Register(WNDCLASSEX& wc);
			static void Unregister(const Tchar* name, HINSTANCE hInstance);
			static bool GetClass(const Tchar* name, WNDCLASSEX& wc);

		private:
			static std::unordered_map<std::string, Cache> s_Classes;
		};
	}
}

#endif