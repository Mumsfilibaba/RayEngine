#include "..\..\Include\System\\Keyboard.h"

#if defined(RE_PLATFORM_ANDROID)

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	bool Keyboard::IsPressed(KEY keyCode)
	{
		//Not implemented on android
		return false;
	}
}

#endif