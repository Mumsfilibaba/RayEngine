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

#include <RayEngine.h>
#include <System\Keyboard.h>

#if defined(RE_PLATFORM_WINDOWS)
#include "Win32KeyCodes.h"

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Keyboard::IsKeyDown(KEY keyCode)
	{
		int32 vk = ReToWin32(keyCode);
		return GetAsyncKeyState(vk) & (1 << 16);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Keyboard::ShowVirtualKeyboardVisible()
	{
		//Not implemented on windows
		return false;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Keyboard::HideVirtualKeyboardVisible()
	{
		//Not implemented on windows
		return false;
	}
}

#endif