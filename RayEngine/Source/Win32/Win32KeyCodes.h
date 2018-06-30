#pragma once

#include "..\..\Include\System\KeyCodes.h"

#if defined (RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	KEY Win32ToRe(int32 keyCode);
	int32 ReToWin32(KEY keyCode);
}

#endif