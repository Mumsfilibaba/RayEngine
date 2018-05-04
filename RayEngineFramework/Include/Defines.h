#pragma once

//Define platform
#ifdef _WIN32 || _WIN64
#define RE_PLATFORM_WINDOWS
#else
#error RayEngine needs to have a platform defined
#endif

//define if we are using ascii or unicode
#ifdef UNICODE
#define TC(x) Lx
#else
#define TC(x) x
#endif