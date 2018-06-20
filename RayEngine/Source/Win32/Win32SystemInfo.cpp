#include "..\..\Include\System\System.h"

#if defined(RE_PLATFORM_WINDOWS)

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN 1
#endif

#include <Windows.h>

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	void QuerySystemInfo(SystemInfo& info)
	{
		info.Platform = PLATFORM_WIN32;
		
		info.ScreenHeight = GetSystemMetrics(SM_CXSCREEN);
		info.ScreenWidth = GetSystemMetrics(SM_CYSCREEN);
	}
}

#endif