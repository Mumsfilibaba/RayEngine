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

#if !defined(RE_UNIQUE_OBJECT)
#define RE_UNIQUE_OBJECT(interface)									\
public:																\
		interface(const interface& other) = delete;					\
		interface& operator=(const interface& other) = delete;		\
		interface(interface&& other) = delete;						\
		interface& operator=(interface&& other) = delete			\

#endif

#if !defined(RE_INTERFACE)
#define RE_INTERFACE(interface) RE_UNIQUE_OBJECT(interface)
#endif

#if !defined(RE_IMPLEMENT_INTERFACE)
#define RE_IMPLEMENT_INTERFACE(interface) RE_UNIQUE_OBJECT(interface)
#endif

#define RE_MAX_RENDERTARGETS 8
#define RE_MAX_VERTEXBUFFERS 32