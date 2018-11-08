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


#include "../../Include/System/System.h"
#if defined(RE_PLATFORM_LINUX)
#include "../../Include/Linux/LinuxWindowImpl.h"

namespace RayEngine
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Display* g_pDisplay = nullptr;
    Atom g_WM_DELETE_WINDOW = 0;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int32 InitializeSystem(int32 systemFlags)
    {
        g_pDisplay = XOpenDisplay(nullptr);
        if (g_pDisplay == nullptr)
        {
            //TODO: Log error
            return 0;
        }
        
        g_WM_DELETE_WINDOW = XInternAtom(g_pDisplay, "WM_DELETE_WINDOW", False);
        if (g_WM_DELETE_WINDOW == None)
        {
            //TODO: Log error
            return 0;
        }

        return 1;
    }


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ReleaseSystem()
    {
        XCloseDisplay(g_pDisplay);
        g_pDisplay = nullptr;
    }


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void QuerySystemDesc(SystemDesc* pDesc)
    {
        if (g_pDisplay == nullptr)
        {
            //TODO: Log error
            return;
        }

        pDesc->Platform = PLATFORM_LINUX;
    
        Screen* pScreen = DefaultScreenOfDisplay(g_pDisplay); 
        pDesc->ScreenWidth = pScreen->width;
        pDesc->ScreenHeight = pScreen->height;
    }


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    IWindow* CreateWindow(WindowDesc* pDesc)
    {
        if (pDesc == nullptr)
        {
            //TODO: Log error
            return nullptr;
        }

        if (g_pDisplay == nullptr)
        {
            //TODO: Log error
            return nullptr;
        }

        return new LinuxWindowImpl(pDesc);
    }


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void SendQuitMessage(int32 exitCode)
    {

    }
}

#endif