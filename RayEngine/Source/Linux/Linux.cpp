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
#include "Linux.h"

#if defined(RE_PLATFORM_LINUX)

namespace RayEngine
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ::Display* g_pDisplay = nullptr;
    int32 g_DisplayRefs = 0;
    ::Atom g_WM_DELETE_WINDOW = 0;
    ::Atom g_MOTIF_WM_HINTS = 0;


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ::Display* GetDisplay()
    {
        if (g_pDisplay == nullptr)
        {
            g_pDisplay = XOpenDisplay(nullptr);
            if (g_pDisplay == nullptr)
            {
                LOG_ERROR("Failed to open a connection to the XServer");
                return nullptr;
            }
        }

        g_DisplayRefs++;
        return g_pDisplay;
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void ReleaseDisplay(::Display* pDisplay)
    {
        if (pDisplay == g_pDisplay)
        {
            g_DisplayRefs--;
            if (g_DisplayRefs <= 0)
            {
                XCloseDisplay(g_pDisplay);
                g_pDisplay = nullptr;
            }
        }
        else
        {
            LOG_WARNING("'pDisplay must be the same display as returned by 'GetDisplay()'");
        }
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ::Atom FindAtom(const std::string& atom)
    {
        ::Display* pDisplay = GetDisplay();
        
        ::Atom result = XInternAtom(pDisplay, atom.c_str(), False);
        ReleaseDisplay(pDisplay);

        return result;
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ::Atom GetXlibAtom(const std::string& atom)
    {
        ::Atom result = 0;
        if (atom == "WM_DELETE_WINDOW")
            result = (g_WM_DELETE_WINDOW == 0) ? (g_WM_DELETE_WINDOW = FindAtom(atom)) : g_WM_DELETE_WINDOW;
        else if (atom == "_MOTIF_WM_HINTS")
            result = (g_MOTIF_WM_HINTS == 0) ? (g_MOTIF_WM_HINTS = FindAtom(atom)) : g_MOTIF_WM_HINTS;

        if (result == 0)
        {
            LOG_ERROR("Atom not found");
        }

        return result;
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void RegisterWindow(::Window window)
    {
        
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void RemoveWindow(::Window window)
    {

    }
}

#endif