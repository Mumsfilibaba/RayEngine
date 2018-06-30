#pragma once

#define RE_STATIC

#if defined(__ANDROID__)
#define RE_PLATFORM_ANDROID
#elif defined(_WIN32) || defined(_WIN64)
#define RE_PLATFORM_WINDOWS
#else
#error RayEngine needs to have a platform defined
#endif

#if defined(_DEBUG)
#define RE_DEBUG
#endif

#if defined(_LIB)
#define RE_LIB
#endif

#if defined(UNICODE)
#define RE_T(x) L##x
#else
#define RE_T(x) x
#endif

#if defined(RE_STATIC)
#define RE_API
#else
#if defined(RE_PLATFORM_WINDOWS)
#define RE_API __declspec(dllexport)
#else
#define RE_API __declspec(dllimport)
#endif
#endif