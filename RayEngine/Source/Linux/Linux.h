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
#include "../../Include/Defines.h"
#include "../../Include/Types.h"

#if defined(RE_PLATFORM_LINUX)
#include <X11/Xlib.h>
#include <string>

namespace RayEngine
{
    ::Display* GetDisplay();

    void ReleaseDisplay(::Display* pDisplay);

    ::Atom GetXlibAtom(const std::string& atom);

    void RegisterWindow(::Window window);

    void RemoveWindow(::Window window);
}

#endif