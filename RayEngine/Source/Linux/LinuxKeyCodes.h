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
#include "../../Include/System/KeyCodes.h"

#if defined(RE_PLATFORM_LINUX)
#include <X11/Xlib.h>
#include <X11/keysym.h>

namespace RayEngine
{
    KEY XKeyEventToKeycode(XKeyEvent* pXEvent);
    KEY XToKeycode(KeySym keysym);
    KeySym KeycodeToX(KEY keycode);
}

#endif