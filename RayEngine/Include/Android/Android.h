#pragma once

#include "..\Defines.h"

#if defined(RE_PLATFORM_ANDROID)

#include "..\Types.h"
#include "..\System\Event.h"

#include <android/native_activity.h>
#include <android/log.h>

#if defined(RE_DEBUG)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "RayEngine App", __VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "RayEngine App", __VA_ARGS__);
#else
#define LOGI(...)
#define LOGE(...)
#endif

/////////////////////////////////////////////////////////////
extern bool AndroidAppHasFocus();

extern void AndroidSetNativeWindowColor(RayEngine::int32 color);
extern void AndroidSetNativeWindowSize(RayEngine::int32 width, RayEngine::int32 height);
extern void AndroidSetNativeWindowFlags(RayEngine::uint32 flags);

extern RayEngine::System::Event AndroidReciveEvent();
extern void AndroidSendEvent(const RayEngine::System::Event& pEvent);

/////////////////////////////////////////////////////////////
extern RayEngine::int32 AndroidGetIntColor(RayEngine::uint8 r, RayEngine::uint8 g, RayEngine::uint8 b, RayEngine::uint8 a);

#endif