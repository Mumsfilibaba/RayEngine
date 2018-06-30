#include "..\..\Include\System\System.h"

#if defined(RE_PLATFORM_ANDROID)
#include "..\..\Include\Android\Android.h"

namespace RayEngine
{
	void QuerySystemInfo(SystemInfo& info)
	{
		info.Platform = PLATFORM_ANDROID;
		
		AndroidGetDisplaySize(info.ScreenWidth, info.ScreenHeight);
	}
}

#endif