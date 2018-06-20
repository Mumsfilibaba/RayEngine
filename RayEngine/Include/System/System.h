#pragma once

#include "..\Defines.h"
#include "..\Types.h"

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	enum PLATFORM
	{
		PLATFORM_UNKNWON = 0,
		PLATFORM_ANDROID = 1,
		PLATFORM_WIN32 = 2
	};



	/////////////////////////////////////////////////////////////
	struct SystemInfo
	{
		PLATFORM Platform = PLATFORM_UNKNWON;
		
		//Resolution of the screen (Android) or the main-monitor (Windows)
		int32 ScreenWidth = 0;
		int32 ScreenHeight = 0;
	};



	/////////////////////////////////////////////////////////////
	void QuerySystemInfo(SystemInfo& info);
}