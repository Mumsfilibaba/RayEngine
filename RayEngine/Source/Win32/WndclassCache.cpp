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

#include "WndclassCache.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace System
	{
		/////////////////////////////////////////////////////////////
		std::unordered_map<std::string, WndclassCache::Cache> WndclassCache::s_Classes;
		


		/////////////////////////////////////////////////////////////
		bool WndclassCache::Register(WNDCLASSEX& wc)
		{
			if (s_Classes.find(wc.lpszClassName) == s_Classes.end())
			{
				Cache cache = {};
				cache.data = wc;
				cache.count = 1;

				SetLastError(0);
				if (RegisterClassEx(&wc) == 0)
				{
					int32 error = GetLastError();
					return false;
				}

				s_Classes[wc.lpszClassName] = cache;
			}
			else
			{
				s_Classes[wc.lpszClassName].count++;
			}

			return true;
		}



		/////////////////////////////////////////////////////////////
		void WndclassCache::Unregister(const Tchar* name, HINSTANCE hInstance)
		{
			if (s_Classes.find(name) != s_Classes.end())
			{
				s_Classes[name].count--;
				if (s_Classes[name].count < 1)
				{
					UnregisterClass(name, hInstance);

					s_Classes.erase(name);
				}
			}
		}



		/////////////////////////////////////////////////////////////
		bool WndclassCache::GetClass(const Tchar* name, WNDCLASSEX& wc)
		{
			if (s_Classes.find(name) != s_Classes.end())
			{
				wc = s_Classes[name].data;
				return true;
			}

			return false;
		}
	}
}

#endif