#include "..\..\Include\Android\Android.h"

#if defined(RE_PLATFORM_ANDROID)

int get_color(RayEngine::uint8 r, RayEngine::uint8 g, RayEngine::uint8 b, RayEngine::uint8 a)
{
	return (a << 24) | (b << 16) | (g << 8) | r;
}

#endif