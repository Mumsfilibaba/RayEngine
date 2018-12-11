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
#include "../../Include/Linux/LinuxWindowImpl.h"

#if defined(RE_PLATFORM_LINUX)

#include "../../Include/Debug/Debug.h"
#include <cstdio>
#include <cstring>
#include "LinuxKeyCodes.h"
#include "Linux.h"
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <X11/keysym.h>

namespace RayEngine
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LinuxWindowImpl::LinuxWindowImpl(const WindowDesc* pDesc)
        : m_pDisplay(nullptr),
        m_XWindow(RE_NULL_WINDOW),
        m_XCursor(0),
        m_XIcon(0),
        m_XIconMask(0),
        m_Events(),
        m_Desc(),
        m_IsMapped(false)
    {
        Create(pDesc, nullptr);
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LinuxWindowImpl::LinuxWindowImpl(const WindowDesc* pDesc, XVisualInfo* pVisualInfo)
        : m_pDisplay(nullptr),
        m_XWindow(RE_NULL_WINDOW),
        m_XCursor(0),
        m_XIcon(0),
        m_XIconMask(0),
        m_Events(),
        m_Desc(),
        m_IsMapped(false)
    {
        Create(pDesc, pVisualInfo);
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LinuxWindowImpl::~LinuxWindowImpl()
    {
        XDestroyWindow(m_pDisplay, m_XWindow);
        m_XWindow = 0;
        
        XFreePixmap(m_pDisplay, m_XIcon);
        m_XIcon = 0;

        XFreePixmap(m_pDisplay, m_XIconMask);
        m_XIconMask = 0;

        ReleaseDisplay(m_pDisplay);
        m_pDisplay = nullptr;

        if (m_Desc.Cursor.pImage != nullptr)
        {
            delete m_Desc.Cursor.pImage;
            m_Desc.Cursor.pImage = nullptr;
        }

        if (m_Desc.pIcon != nullptr)
        {
            delete m_Desc.pIcon;
            m_Desc.pIcon = nullptr;
        }
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void LinuxWindowImpl::Show() const
    {
        if (!m_IsMapped)
        {
            XMapWindow(m_pDisplay, m_XWindow);
            XFlush(m_pDisplay);

            m_IsMapped = true;

            CheckEvents();
        }
    }
	

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void LinuxWindowImpl::Close() const
    {
       if (m_IsMapped)
        {
            XUnmapWindow(m_pDisplay, m_XWindow);
            XFlush(m_pDisplay);              

            m_IsMapped = false;
            
            CheckEvents();
        }
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	bool LinuxWindowImpl::PeekEvent(Event* pEvent) const
    {
        CheckEvents();
        if (m_Events.empty())
        {
            return false;
        }

        *pEvent = m_Events.front();
        m_Events.pop();
        return true;
    }
		

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void LinuxWindowImpl::GetEvent(Event* pEvent) const
    {
        while (m_Events.empty()) 
        { 
            CheckEvents();
        }

        *pEvent = m_Events.front();
        m_Events.pop();
    }
				

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void LinuxWindowImpl::SetCursor(const Image* pCursor, const Math::Point& hotspot)
    {
        if (pCursor == nullptr)
        {
            return;
        }

        if (pCursor->GetFormat() != FORMAT_R8G8B8A8_UNORM &&
            pCursor->GetFormat() != FORMAT_B8G8R8A8_UNORM)
        {
            LOG_ERROR("Cursor has wrong pixelformat");
            return;
        }

        m_Desc.Cursor.pImage = new Image(*pCursor);
        m_Desc.Cursor.Hotspot = hotspot;

        uint8* pPixels = nullptr;
        m_Desc.Cursor.pImage->GetPixels(reinterpret_cast<void**>(&pPixels));

        int32 width = m_Desc.Cursor.pImage->GetWidth();
        int32 height = m_Desc.Cursor.pImage->GetHeight();
        int32 size = ((width + 7) / 8) * height;
        std::vector<uint8> bitmask(size, 0);
        std::vector<uint8> pixels(size, 0);

        int32 pixel = 0;
        int32 byte = 0;
        int32 bit = 0;
        int32 intensity = 0;
        uint8 opacity = 0;
        uint8 mask = 0;

        for (int32 y = 0; y < height; y++)
        {
            for (int32 x = 0; x < width; x++)
            {
                pixel = (y * width) + x;
                byte = pixel / 8;
                bit = x % 8;

                opacity = pPixels[(pixel * 4) + 3] > 0 ? 1 : 0;
                bitmask[byte] |= opacity << bit;

                intensity = pPixels[(pixel * 4) + 0] + pPixels[(pixel * 4) + 1] + pPixels[(pixel * 4) + 2];
                mask = intensity > 380 ? 1 : 0;
                pixels[byte] |= mask << bit;
            }
        }

        ::Window root = DefaultRootWindow(m_pDisplay);
        ::Pixmap maskMap = XCreateBitmapFromData(m_pDisplay, root, reinterpret_cast<const char*>(bitmask.data()), width, height);
        ::Pixmap pixelMap = XCreateBitmapFromData(m_pDisplay, root, reinterpret_cast<const char*>(pixels.data()), width, height);

        XColor foreGroundCol = {};
        foreGroundCol.red = -1;
        foreGroundCol.green = -1;
        foreGroundCol.blue = -1;

        XColor backGroundCol = {};
        backGroundCol.red = 0;
        backGroundCol.green = 0;
        backGroundCol.blue = 0;

        if (m_XCursor != 0)
        {
            XFreeCursor(m_pDisplay, m_XCursor);
            m_XCursor = 0;
        }

        m_XCursor = XCreatePixmapCursor(m_pDisplay, pixelMap, maskMap, &foreGroundCol, &backGroundCol, hotspot.x, hotspot.y);
        XFreePixmap(m_pDisplay, maskMap);
        XFreePixmap(m_pDisplay, pixelMap);

        XDefineCursor(m_pDisplay, m_XWindow, m_XCursor);
    }
    

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void LinuxWindowImpl::SetIcon(const Image* pIcon)
    {
        if (pIcon == nullptr)
        {
            return;
        }


        if (pIcon->GetFormat() != FORMAT_R8G8B8A8_UNORM &&
            pIcon->GetFormat() != FORMAT_B8G8R8A8_UNORM)
        {
            LOG_ERROR("Icon has wrong pixelformat");
            return;
        }

        m_Desc.pIcon = new Image(*pIcon);
                
        uint8* pPixels = nullptr;
        m_Desc.pIcon->GetPixels(reinterpret_cast<void**>(&pPixels));
 
        int32 width = m_Desc.pIcon->GetWidth();
        int32 height = m_Desc.pIcon->GetHeight();

        if (pIcon->GetFormat() == FORMAT_R8G8B8A8_UNORM)
        {
            uint8 component = 0;
            for (int32 i = 0; i < width * height * 4; i += 4)
            {
                component = pPixels[i + 0]; 
                pPixels[i + 0] = pPixels[i + 2];
                pPixels[i + 2] = component;
            }
        }

        int32 screen = DefaultScreen(m_pDisplay);
        uint32 depth = DefaultDepth(m_pDisplay, screen); 
        ::Visual* pVisual = DefaultVisual(m_pDisplay, screen);

        if (m_XIcon != 0)
        {
            XFreePixmap(m_pDisplay, m_XIcon);
            m_XIcon = 0;
        }

        if (m_XIconMask != 0)
        {
            XFreePixmap(m_pDisplay, m_XIconMask);
            m_XIconMask = 0;
        }

        ::Window rootWindow = DefaultRootWindow(m_pDisplay);
        m_XIcon = XCreatePixmap(m_pDisplay, rootWindow, width, height, depth);
        if (m_XIcon == 0)
        {
            LOG_ERROR("Failed to create a icon pixmap");
            return;
        }

        XImage* pImage = XCreateImage(m_pDisplay, pVisual, depth, ZPixmap, 0, reinterpret_cast<char*>(pPixels), width, height, 32, 0);
        if (pImage == nullptr)
        {
            LOG_ERROR("Failed to create image");
            return;
        }

        XGCValues values = {};
        GC gc = XCreateGC(m_pDisplay, m_XIcon, 0, &values);
        XPutImage(m_pDisplay, m_XIcon, gc, pImage, 0, 0, 0, 0, width, height);

        XFreeGC(m_pDisplay, gc);

        XWMHints* pWMHints = XAllocWMHints();
        if (pWMHints == nullptr)
        {
            LOG_ERROR("'XAllocWMHints' failed");
            return;
        }

        int32 maskWidth = (width + 7) / 8;
        std::vector<uint8> maskBuffer(maskWidth * height, 0);
        for (int32 y = 0; y < height; y++)
        {
            for (int32 x = 0; x < maskWidth; x++)
            {
                for (int32 i = 0; i < 8; i++)
                {
                    if ((x * 8) + i < width)
                    {
                        uint8 opacity = (pPixels[((x * 8) + i + (y * width)) * 4 + 3] > 0) ? 1 : 0;
                        maskBuffer[x + (y * maskWidth)] |= (opacity << i);
                    }
                }
            }
        }

        XDestroyImage(pImage);
        m_XIconMask = XCreatePixmapFromBitmapData(m_pDisplay, m_XWindow, reinterpret_cast<char*>(maskBuffer.data()), width, height, 1, 0, 1);

        pWMHints->flags = IconPixmapHint | IconMaskHint;
        pWMHints->icon_pixmap = m_XIcon;
        pWMHints->icon_mask = m_XIconMask;
        
        XSetWMHints(m_pDisplay, m_XWindow, pWMHints);
        XFree(pWMHints);
        XFlush(m_pDisplay);
    }
        

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void LinuxWindowImpl::SetTitle(const std::string& title)
    {
        XStoreName(m_pDisplay, m_XWindow, title.c_str());
        m_Desc.Title = title;
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void LinuxWindowImpl::SetBackground(uint8 r, uint8 g, uint8 b)
    {
        Colormap colormap = XDefaultColormap(m_pDisplay, 0);
        
        XColor bg = {};
        bg.flags = DoRed | DoGreen | DoBlue;
        bg.red = (r / 255.0f) * 0xFFFF;
        bg.green = (g / 255.0f) * 0xFFFF;
        bg.blue = (b / 255.0f) * 0xFFFF;

        XAllocColor(m_pDisplay, colormap, &bg);

        XSetWindowAttributes attributes = {};
        attributes.background_pixel = bg.pixel;

        int32 attributesMask = CWBackPixel;

        if (XChangeWindowAttributes(m_pDisplay, m_XWindow, attributesMask, &attributes))
        {
            m_Desc.BackgroundColor.r = r;
            m_Desc.BackgroundColor.g = g;
            m_Desc.BackgroundColor.b = b;
        }
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
        *pDesc = m_Desc;
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void LinuxWindowImpl::Destroy()
    {
        delete this;   
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void LinuxWindowImpl::GetNativeWindowHandle(NativeWindowHandle* pHandle) const
    {
        *pHandle = static_cast<::Window>(m_XWindow);
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void LinuxWindowImpl::Create(const WindowDesc* pDesc, XVisualInfo* pVisualInfo)
    {
        m_pDisplay = GetDisplay();
        ::Window root = DefaultRootWindow(m_pDisplay);
        
        m_Desc = *pDesc;

        int32 depth = 0;
        Visual* pVisual = nullptr;
        Colormap colormap = 0;
        if (pVisualInfo == nullptr)
        {
            depth = DefaultDepth(m_pDisplay, 0);
            pVisual = DefaultVisual(m_pDisplay, 0);
            colormap =  XDefaultColormap(m_pDisplay, 0);
        }
        else
        {
            depth = pVisualInfo->depth;
            pVisual = pVisualInfo->visual;
            colormap = XCreateColormap(m_pDisplay, root, pVisualInfo->visual, AllocNone);
        }
        
        XColor bg = {};
        bg.flags = DoRed | DoGreen | DoBlue;
        bg.red = (pDesc->BackgroundColor.r / 255.0f) * 0xFFFF;
        bg.green = (pDesc->BackgroundColor.g / 255.0f) * 0xFFFF;
        bg.blue = (pDesc->BackgroundColor.b / 255.0f) * 0xFFFF;

        XAllocColor(m_pDisplay, colormap, &bg);

        XSetWindowAttributes attributes = {};
        attributes.colormap = colormap;
        attributes.background_pixel = bg.pixel;
        attributes.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask |
                                FocusChangeMask | ButtonPressMask | ButtonReleaseMask |
                                ButtonMotionMask | PointerMotionMask;

        int32 attributesMask = CWColormap |  CWBackPixel | CWEventMask;

        m_XWindow = XCreateWindow(m_pDisplay, root, m_Desc.x, m_Desc.y, m_Desc.Width, m_Desc.Height, 5, depth, InputOutput, pVisual, attributesMask, &attributes);
        
        ::Atom WM_DELETE_WINDOW = GetXlibAtom("WM_DELETE_WINDOW");
        XSetWMProtocols(m_pDisplay, m_XWindow, &WM_DELETE_WINDOW, 1);

        SetTitle(m_Desc.Title);
        SetClassHints();
        SetIcon(m_Desc.pIcon);
        SetCursor(m_Desc.Cursor.pImage, m_Desc.Cursor.Hotspot);
        SetStyle();
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
        {
            ::Atom WM_DELETE_WINDOW = GetXlibAtom("WM_DELETE_WINDOW");
            if (static_cast<Atom>(pXEvent->xclient.data.l[0]) == WM_DELETE_WINDOW)
            {
                pEvent->Type = EVENT_TYPE_CLOSE;
            }
            return;
        }

        case ConfigureNotify:
            if (pXEvent->xconfigure.width != m_Desc.Width || pXEvent->xconfigure.height != m_Desc.Height)
            {
                pEvent->Type = EVENT_TYPE_RESIZE;
                pEvent->Resize.Width = pXEvent->xconfigure.width;
                pEvent->Resize.Height = pXEvent->xconfigure.height;
                pEvent->Resize.Type = EVENT_RESIZE_NEW_SIZE;

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

            {
                static XComposeStatus status = {};
                static char buffer[16];

                XLookupString(&pXEvent->xkey, buffer, sizeof(buffer), NULL, &status);

                Event event = {};
                event.Type = EVENT_TYPE_KEYCHAR;
                event.KeyChar.UnicodeChar = static_cast<uint32>(buffer[0]);
                m_Events.push(event);
            }
            return;
        
        case KeyRelease:
            pEvent->Type = EVENT_TYPE_KEYRELEASED;
            pEvent->Key.KeyCode = XKeyEventToKeycode(&pXEvent->xkey);
            return;

        case FocusOut:
        case FocusIn:
            pEvent->Type = EVENT_TYPE_FOCUSCHANGED;
            pEvent->FocusChanged.HasFocus = pXEvent->type == FocusIn;
            return;

        case MotionNotify:
            pEvent->Type = EVENT_TYPE_MOUSEMOVE;
            pEvent->MouseMove.Position.x = pXEvent->xmotion.x;
            pEvent->MouseMove.Position.y = pXEvent->xmotion.y;
            return;

        case ButtonPress:
            if (pXEvent->xbutton.button == Button1 || pXEvent->xbutton.button == Button2 ||
                pXEvent->xbutton.button == Button3 || pXEvent->xbutton.button == 8 ||
                pXEvent->xbutton.button == 9)
            {
                pEvent->Type = EVENT_TYPE_MOUSEPRESSED;
                switch(pXEvent->xbutton.button)
                {
                case Button1: pEvent->MouseButton.Button = MOUSEBUTTON_LEFT; break;
                case Button2: pEvent->MouseButton.Button = MOUSEBUTTON_MIDDLE; break;
                case Button3: pEvent->MouseButton.Button = MOUSEBUTTON_RIGHT; break;
                case 8: pEvent->MouseButton.Button = MOUSEBUTTON_FORWARD; break;
                case 9: pEvent->MouseButton.Button = MOUSEBUTTON_BACK; break;
                default: pEvent->MouseButton.Button = MOUSEBUTTON_UNKNOWN; break;
                }
            }
            return;

        case ButtonRelease:
            if (pXEvent->xbutton.button == Button1 || pXEvent->xbutton.button == Button2 ||
                pXEvent->xbutton.button == Button3 || pXEvent->xbutton.button == 8 ||
                pXEvent->xbutton.button == 9)
            {
                pEvent->Type = EVENT_TYPE_MOUSERELEASED;
                switch(pXEvent->xbutton.button)
                {
                case Button1: pEvent->MouseButton.Button = MOUSEBUTTON_LEFT; break;
                case Button2: pEvent->MouseButton.Button = MOUSEBUTTON_MIDDLE; break;
                case Button3: pEvent->MouseButton.Button = MOUSEBUTTON_RIGHT; break;
                case 8: pEvent->MouseButton.Button = MOUSEBUTTON_FORWARD; break;
                case 9: pEvent->MouseButton.Button = MOUSEBUTTON_BACK; break;
                default: pEvent->MouseButton.Button = MOUSEBUTTON_UNKNOWN; break;
                }
            }
            else if (pXEvent->xbutton.button == 4)
            {
                pEvent->Type = EVENT_TYPE_MOUSESCROLL;
                pEvent->MouseScroll.Delta = 1.0f;
            }
            else if (pXEvent->xbutton.button == 5)
            {
                pEvent->Type = EVENT_TYPE_MOUSESCROLL;
                pEvent->MouseScroll.Delta = -1.0f;
            }
            else
            {
                pEvent->Type = EVENT_TYPE_UNKNOWN;
            }     
            return;
        
        case MapNotify:
        case UnmapNotify:
        default:
            pEvent->Type = EVENT_TYPE_UNKNOWN;
            return;
        }
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Bool EventFilter(::Display* pDisplay, XEvent* pEvent, XPointer userData)
    {
        return pEvent->xany.window == reinterpret_cast<::Window>(userData);
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void LinuxWindowImpl::CheckEvents() const
    {
        Event event = {};
        XEvent xevent = {};

        while(XCheckIfEvent(m_pDisplay, &xevent, &EventFilter, reinterpret_cast<XPointer>(m_XWindow)))
        {
            ConvertXEvent(&xevent, &event);
            if (event.Type != EVENT_TYPE_UNKNOWN)
            {
                m_Events.push(event);
            }
        }
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void LinuxWindowImpl::SetClassHints()
    {
        XClassHint* pClassHints = XAllocClassHint();
        if (pClassHints == nullptr)
        {
            LOG_ERROR("'XAllocClassHint()' failed");
            return;
        }

        std::vector<char> buffer(256, 0);
        
        FILE* pFile = fopen("/proc/self/cmdline", "r");
        if (pFile != nullptr)
        {
            fread(buffer.data(), sizeof(char), 256, pFile);

            pClassHints->res_name = (strrchr(buffer.data(), '/') + 1);

            fclose(pFile);
        }
        else
        {
            strcpy(buffer.data(), "RayEngine");
            pClassHints->res_name = buffer.data();
        }

        std::vector<char> className(m_Desc.Title.size(), 0);
        strcpy(className.data(), m_Desc.Title.c_str());

        pClassHints->res_class = className.data();

        XSetClassHint(m_pDisplay, m_XWindow, pClassHints);

        XFree(pClassHints);
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void LinuxWindowImpl::SetStyle()
    {
        ::Atom _MOTIF_WM_HINTS = GetXlibAtom("_MOTIF_WM_HINTS");
        if (_MOTIF_WM_HINTS != 0)
        {
            struct MotifWMHints
            {
                ulong Flags;
                ulong Functions;
                ulong Decorations;
                long InputMode;
                ulong State;
            };

            enum MWM_HINTS : int32
            {
                MWM_HINTS_FUNCTIONS = (1 << 0),
                MWM_HINTS_DECORATIONS = (1 << 1),
            };

            enum MWM_FUNC : int32
            {
                MWM_FUNC_RESIZE = (1L << 1),
                MWM_FUNC_MOVE = (1L << 2),
                MWM_FUNC_MINIMIZE = (1L << 3),
                MWM_FUNC_MAXIMIZE = (1L << 4),
                MWM_FUNC_CLOSE = (1L << 5),
                MWM_FUNC_ALL =  MWM_FUNC_RESIZE | MWM_FUNC_MOVE | MWM_FUNC_MINIMIZE |
                                MWM_FUNC_MAXIMIZE | MWM_FUNC_CLOSE,
            };

            enum MWM_DECOR : int32
            {
                MWM_DECOR_BORDER = (1L << 1),
                MWM_DECOR_RESIZE = (1L << 2),
                MWM_DECOR_TITLE = (1L << 3),
                MWM_DECOR_MENU = (1L << 4),
                MWM_DECOR_MINIMIZE = (1L << 5),
                MWM_DECOR_MAXIMIZE = (1L << 6),
                MWM_DECOR_ALL = MWM_DECOR_BORDER | MWM_DECOR_RESIZE | MWM_DECOR_TITLE |
                                MWM_DECOR_MENU | MWM_DECOR_MINIMIZE | MWM_DECOR_MAXIMIZE
            };

            MotifWMHints hints = {};
            memset(&hints, 0, sizeof(MotifWMHints));
            hints.Flags = MWM_HINTS_FUNCTIONS | MWM_HINTS_DECORATIONS;

            if (m_Desc.Style & WINDOWSTYLE_RESIZEABLE)
            {
                hints.Functions |= MWM_FUNC_RESIZE | MWM_FUNC_MOVE;
                hints.Decorations |= MWM_DECOR_RESIZE;
            }
            if (m_Desc.Style & WINDOWSTYLE_MAXIMIZABLE)
            {
                hints.Functions |= MWM_FUNC_MAXIMIZE | MWM_FUNC_MOVE;
                hints.Decorations |= MWM_DECOR_MAXIMIZE;
            }
            if (m_Desc.Style & WINDOWSTYLE_MINIMIZABLE)
            {
                hints.Functions |= MWM_FUNC_MINIMIZE | MWM_FUNC_MOVE;
                hints.Decorations |= MWM_DECOR_MINIMIZE;
            }
            if (m_Desc.Style & WINDOWSTYLE_CLOSE)
            {
                hints.Functions |= MWM_FUNC_CLOSE | MWM_FUNC_MOVE;
                hints.Decorations |= MWM_DECOR_MENU;
            }
            if (m_Desc.Style & WINDOWSTYLE_TITLEBAR)
            {
                hints.Functions |= MWM_FUNC_MOVE;
                hints.Decorations |= MWM_DECOR_TITLE;
            }
            if (m_Desc.Style == WINDOWSTYLE_BORDERLESS)
            {
                hints.Functions = 0;
                hints.Decorations = 0;
            }

            XChangeProperty(m_pDisplay, m_XWindow, _MOTIF_WM_HINTS, _MOTIF_WM_HINTS, 32, PropModeReplace, reinterpret_cast<const unsigned char*>(&hints), 5);
        }

        if (!(m_Desc.Style & WINDOWSTYLE_RESIZEABLE))
        {
            XSizeHints* pSizeHints = XAllocSizeHints();
            pSizeHints->flags = PMinSize | PMaxSize;
            pSizeHints->min_width = pSizeHints->max_width = m_Desc.Width;
            pSizeHints->min_height = pSizeHints->max_height = m_Desc.Height;

            XSetWMNormalHints(m_pDisplay, m_XWindow, pSizeHints);
            XFree(pSizeHints);
        }
    }
}

#endif