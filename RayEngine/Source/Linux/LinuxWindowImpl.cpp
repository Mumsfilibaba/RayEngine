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
#include "../../Include/Linux/LinuxWindowImpl.h"
#include "LinuxKeyCodes.h"

#if defined(RE_PLATFORM_LINUX)

#include <iostream> 

namespace RayEngine
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LinuxWindowImpl::LinuxWindowImpl(const WindowDesc* pDesc)
        : m_XWindow(0),
        m_Desc(),
        m_IsVisible(false)
    {
        Create(pDesc);
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LinuxWindowImpl::~LinuxWindowImpl()
    {
        XDestroyWindow(g_pDisplay, m_XWindow);
        m_XWindow = 0;
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void LinuxWindowImpl::Show() const
    {
        if (!m_IsVisible)
        {
            XMapWindow(g_pDisplay, m_XWindow);
            
            XFlush(g_pDisplay);

            m_IsVisible = true;
        }
    }
	

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	bool LinuxWindowImpl::PeekEvent(Event* pEvent) const
    {
        return false;
    }
		

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void LinuxWindowImpl::GetEvent(Event* pEvent) const
    {
        XEvent event = {};
        XNextEvent(g_pDisplay, &event);
        if (event.xany.window == m_XWindow)
        {
            ConvertXEvent(&event, pEvent);
        }
        else
        {
            XPutBackEvent(g_pDisplay, &event);
        }
    }
				

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void LinuxWindowImpl::SetCursor(const Bitmap& cursor, int32 hotspotX, int32 hotspotY)
    {
        
    }
    

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void LinuxWindowImpl::SetIcon(const Bitmap& icon)
    {
        
    }
    

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void LinuxWindowImpl::SetBackground(uint8 r, uint8 g, uint8 b)
    {
        
    }
    

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int32 LinuxWindowImpl::GetWidth() const
    {
        return m_Desc.Width;
    }
    

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int32 LinuxWindowImpl::GetHeight() const
    {
        return m_Desc.Height;
    }
    

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void LinuxWindowImpl::GetDesc(WindowDesc* pDesc) const
    {

    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void LinuxWindowImpl::Destroy()
    {
        delete this;   
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void LinuxWindowImpl::Create(const WindowDesc* pDesc)
    {
        m_Desc = *pDesc;

        XColor bg = {};
        bg.flags = DoRed | DoGreen | DoBlue;
        bg.red = (pDesc->BackgroundColor.r / 255.0f) * 0xFFFF;
        bg.green = (pDesc->BackgroundColor.g / 255.0f) * 0xFFFF;
        bg.blue = (pDesc->BackgroundColor.b / 255.0f) * 0xFFFF;

        Colormap colormap = DefaultColormap(g_pDisplay, 0);
        if (XAllocColor(g_pDisplay, colormap, &bg))
        {
            std::cout << "bgPixel: " << bg.pixel << std::endl;
        }


        XSetWindowAttributes attributes = {};
        attributes.background_pixel = bg.pixel;
        attributes.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask;

        int32 attributesMask = CWBackPixel | CWEventMask;

        int32 depth = DefaultDepth(g_pDisplay, 0);
        Visual* pVisual = DefaultVisual(g_pDisplay, 0);

        Create(&attributes, pVisual, depth, attributesMask);
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void LinuxWindowImpl::Create(XSetWindowAttributes* pAttributes, Visual* pVisual, int32 depth, int32 attributesMask)
    {
        Window root = XRootWindow(g_pDisplay, 0);
        m_XWindow = XCreateWindow(g_pDisplay, root, 0, 0, m_Desc.Width, m_Desc.Height, 5, depth, InputOutput, pVisual, attributesMask, pAttributes);
        
        XStoreName(g_pDisplay, m_XWindow, m_Desc.Title);
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void LinuxWindowImpl::ConvertXEvent(XEvent* pXEvent, Event* pEvent) const
    {
        switch (pXEvent->type)
        {
        case DestroyNotify:
            pEvent->Type = EVENT_TYPE_CLOSE;
            return;

        case ClientMessage:
            if (static_cast<Atom>(pXEvent->xclient.data.l[0]) == g_WM_DELETE_WINDOW)
            {
                pEvent->Type = EVENT_TYPE_CLOSE;
            }
            return;

        case ConfigureNotify:
            if (pXEvent->xconfigure.width != m_Desc.Width || pXEvent->xconfigure.height != m_Desc.Height)
            {
                pEvent->Type = EVENT_TYPE_RESIZE;
                pEvent->Resize.Width = pXEvent->xconfigure.width;
                pEvent->Resize.Height = pXEvent->xconfigure.height;

                m_Desc.Width = pXEvent->xconfigure.width;
                m_Desc.Height = pXEvent->xconfigure.height;
            }
            else
            {
                pEvent->Type = EVENT_TYPE_UNKNOWN;
            }

            return;

        case KeyPress:
            pEvent->Type = EVENT_TYPE_KEYPRESSED;
            pEvent->Key.KeyCode = XKeyEventToKeycode(&pXEvent->xkey);
            return;
        
        case KeyRelease:
            pEvent->Type = EVENT_TYPE_KEYRELEASED;
            pEvent->Key.KeyCode = XKeyEventToKeycode(&pXEvent->xkey);
            return;
        
        case MapNotify:
        default:
            pEvent->Type = EVENT_TYPE_UNKNOWN;
            return;
        }
    }
}

#endif