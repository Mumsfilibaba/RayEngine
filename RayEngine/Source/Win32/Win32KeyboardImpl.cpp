#include "..\..\Include\System\Keyboard.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "Win32KeyCodes.h"

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN 1
#endif

#include <Windows.h>

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	bool Keyboard::IsPressed(KEY keyCode)
	{
		int32 vk = ReToWin32(keyCode);
		return GetAsyncKeyState(vk) & (1 << 16);
	}



	/////////////////////////////////////////////////////////////
	void Keyboard::SetScreenKeyboardVisible(bool visible)
	{
		//Not implemented on windows
	}
}

#endif