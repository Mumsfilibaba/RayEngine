#pragma once

#include "..\..\Include\System\KeyCodes.h"

#if defined (RE_PLATFORM_ANDROID)

namespace RayEngine
{
	KEY AndroidToRe(int32 keyCode);
	int32 ReToAndroid(KEY keyCode);
}

#endif