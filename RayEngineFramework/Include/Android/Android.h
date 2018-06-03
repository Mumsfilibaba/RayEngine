#pragma once

#include "..\Defines.h"

#if defined(RE_PLATFORM_ANDROID)

#include "..\Types.h"
#include <android/native_activity.h>
#include <android/log.h>

#if defined(RE_DEBUG)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "RayEngine App", __VA_ARGS__);
#else
#define LOGI(...)
#endif

/////////////////////////////////////////////////////////////

enum ANDROID_EVENT : RayEngine::int32
{
	ANDROID_EVENT_UNKNOWN = 0,
	ANDROID_EVENT_DESTROY = -1,
	ANDROID_EVENT_WINDOW_CREATE = 1,
	ANDROID_EVENT_WINDOW_DESTROYED = 2,
	ANDROID_EVENT_WINDOW_RESIZE = 3,
	ANDROID_EVENT_WINDOW_REPAINT = 4,
	ANDROID_EVENT_APP_START = 5,
	ANDROID_EVENT_APP_STOP = 6,
	ANDROID_EVENT_APP_PAUSE = 7,
	ANDROID_EVENT_APP_RESUME = 8,
	ANDROID_EVENT_LOW_MEMORY = 8,
};

/////////////////////////////////////////////////////////////

extern ANativeActivity* get_activity();
extern void set_native_window_color(RayEngine::int32 color);
extern void set_native_window_size(RayEngine::int32 width, RayEngine::int32 height);
extern void set_native_window_flags(RayEngine::uint32 addflags, RayEngine::uint32 removeFlags);

extern ANDROID_EVENT recive_event();
extern void send_event(ANDROID_EVENT event);

/////////////////////////////////////////////////////////////

extern int get_color(RayEngine::uint8 r, RayEngine::uint8 g, RayEngine::uint8 b, RayEngine::uint8 a);

#endif