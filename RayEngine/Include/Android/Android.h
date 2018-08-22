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
#include "..\Defines.h"

#if defined(RE_PLATFORM_ANDROID)

#include "..\Types.h"
#include "..\System\Event.h"

#include <android\native_activity.h>
#include <android\log.h>

#if defined(RE_DEBUG)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "RayEngine App", __VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "RayEngine App", __VA_ARGS__);
#else
#define LOGI(...)
#define LOGE(...)
#endif

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	extern void SetNativeActivity(ANativeActivity* pActivity);
	extern ANativeActivity* GetNativeActivity();
	extern void AndroidGetDisplaySize(int32& rWidth, int32& rHeight);
	extern int32 AndroidGetSDKVersion();
	extern bool AndroidAppHasFocus();

	extern void AndroidSetNativeWindowColor(int32 color);
	extern void AndroidSetNativeWindowSize(int32 width, int32 height);
	extern void AndroidSetNativeWindowFlags(uint32 flags);

	extern System::Event AndroidReciveEvent();
	extern void AndroidSendEvent(const System::Event& rEvent);

	/////////////////////////////////////////////////////////////
	extern int32 GetAndroidColor(uint8 r, uint8 g, uint8 b, uint8 a);
}

#endif