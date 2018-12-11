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

#include "RayEngine.h"
#include "../../Include/System/Keyboard.h"

#if defined(RE_PLATFORM_LINUX)
#include "LinuxKeyCodes.h"
#include "../../Include/Utilities/StringUtilities.h"
#include "Linux.h"

#include <iostream>

namespace RayEngine
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool Keyboard::IsKeyDown(KEY keyCode)
    {
        ::Display* pDisplay = GetDisplay();

        if (pDisplay == nullptr)
        {
            return false;
        }

        KeySym xKeysym = KeycodeToX(keyCode);
        KeyCode xKeycode = XKeysymToKeycode(pDisplay, xKeysym);
        if (xKeycode != 0)
        {
            char keyState[32];
            XQueryKeymap(pDisplay, keyState);

            return (keyState[xKeycode / 8] & (1 << (xKeycode % 8))) != 0;
        }

        return false;
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Keyboard::ShowVirtualKeyboardVisible()
    {
        return false;
    }
	

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Keyboard::HideVirtualKeyboardVisible()
    {
        return false;
    }
}

#endif