#include "..\..\Include\System\System.h"

#if defined(RE_PLATFORM_ANDROID)

namespace RayEngine
{
	void QuerySystemInfo(SystemInfo& info)
	{
		info.Platform = PLATFORM_ANDROID;
	}
}

#endif