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

#include "../../Include/System/Mouse.h"

#if defined(RE_PLATFORM_LINUX)
#include "../../Include/Linux/LinuxWindowImpl.h"
#include "Linux.h"

namespace RayEngine
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Mouse::SetPosition(const Math::Point& position)
    {
        ::Display* pDisplay = GetDisplay();

        XWarpPointer(pDisplay, None, DefaultRootWindow(pDisplay), 0, 0, 0, 0, position.x, position.y);
        XFlush(pDisplay);

        ReleaseDisplay(pDisplay);
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Mouse::SetPosition(const Math::Point& position, const IWindow* pWindow)
    {
        const LinuxWindowImpl* pLinuxWindow = reinterpret_cast<const LinuxWindowImpl*>(pWindow);
        if (pLinuxWindow == nullptr)
        {
            return;
        }

        ::Window xWindow = pLinuxWindow->GetXWindow();
        if (xWindow != 0)
        {
            ::Display* pDisplay = GetDisplay();

            XWarpPointer(pDisplay, None, xWindow, 0, 0, 0, 0, position.x, position.y);
            XFlush(pDisplay);

            ReleaseDisplay(pDisplay);
        }
    }
		

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Math::Point Mouse::GetPosition()
    {
        ::Display* pDisplay = GetDisplay();

        ::Window rootWindow = 0;
        ::Window childWindow = 0;
        int32 winX = 0;
        int32 winY = 0;
        int32 rootX = 0;
        int32 rootY = 0;
        uint32 buttons = 0;

        XQueryPointer(pDisplay, DefaultRootWindow(pDisplay), &rootWindow, &childWindow, &rootX, &rootY, &winX, &winY, &buttons);

        ReleaseDisplay(pDisplay);
        return Math::Point(winX, winX);
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	Math::Point Mouse::GetPosition(const IWindow* pWindow)
    {
        const LinuxWindowImpl* pLinuxWindow = reinterpret_cast<const LinuxWindowImpl*>(pWindow);
        if (pLinuxWindow == nullptr)
        {
            return Math::Point();
        }

        ::Window xWindow = pLinuxWindow->GetXWindow();
        if (xWindow != 0)
        {
            ::Display* pDisplay = GetDisplay();

            ::Window rootWindow = 0;
            ::Window childWindow = 0;
            int32 winX = 0;
            int32 winY = 0;
            int32 rootX = 0;
            int32 rootY = 0;
            uint32 buttons = 0;

            XQueryPointer(pDisplay, xWindow, &rootWindow, &childWindow, &rootX, &rootY, &winX, &winY, &buttons);

            ReleaseDisplay(pDisplay);
            return Math::Point(winX, winY);
        }
        else
        {
            return Math::Point();
        }
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Mouse::IsButtonDown(MOUSEBUTTON button)
    {
        ::Display* pDisplay = GetDisplay();

        ::Window rootWindow = 0;
        ::Window childWindow = 0;
        int32 winX = 0;
        int32 winY = 0;
        int32 rootX = 0;
        int32 rootY = 0;
        uint32 buttons = 0;

        XQueryPointer(pDisplay, DefaultRootWindow(pDisplay), &rootWindow, &childWindow, &rootX, &rootY, &winX, &winY, &buttons);

        ReleaseDisplay(pDisplay);
        
        if (button == MOUSEBUTTON_LEFT)
            return buttons & Button1Mask;
        else if (button == MOUSEBUTTON_MIDDLE)
            return buttons & Button2Mask;
        else if (button == MOUSEBUTTON_RIGHT)
            return buttons & Button3Mask;

        return false;
    }
}

#endif