#pragma once

//Define platform
#if defined(_WIN32) || defined(_WIN64)
#define RE_PLATFORM_WINDOWS
#elif defined(_ANDROID)
#define RE_PLATFORM_ANDROID
#else
#error RayEngine needs to have a platform defined
#endif

//Define debugbuild
#if defined(_DEBUG)
#define RE_DEBUG
#endif

//define if we are using ascii or unicode
#if defined(UNICODE)
#define RE_T(x) Lx
#else
#define RE_T(x) x
#endif

//Define DLL-Export
#if defined(RE_PLATFORM_WINDOWS)
#if defined(RE_STATIC)
#define RE_API
#else
#define RE_API __declspec(dllexport)
#endif
#endif