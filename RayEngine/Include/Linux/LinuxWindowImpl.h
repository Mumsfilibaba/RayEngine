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

#include "../System/Window.h" 

#if defined(RE_PLATFORM_LINUX)
#include "../System/KeyCodes.h"
#include <queue>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

namespace RayEngine
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class LinuxWindowImpl final : public IWindow
    {
        RE_IMPLEMENT_INTERFACE(LinuxWindowImpl);

    public:
        LinuxWindowImpl(const WindowDesc* pDesc);
        LinuxWindowImpl(const WindowDesc* pDesc, XVisualInfo* pVisualInfo);
        ~LinuxWindowImpl();

        inline Window GetXWindow() const 
        {
            return m_XWindow;
        }

		void Show() const override final;

        void Close() const override final;
		
		bool PeekEvent(Event* pEvent) const override final;
		
        void GetEvent(Event* pEvent) const override final;
		
        void SetCursor(const Image* pCursor, const Math::Point& hotspot) override final;
		
        void SetIcon(const Image* pIcon) override final;

        void SetTitle(const std::string& title) override final;
        
        void SetBackground(uint8 r, uint8 g, uint8 b) override final;
		
        int32 GetWidth() const override final;
		
        int32 GetHeight() const override final;
		
        void GetDesc(WindowDesc* pDesc) const override final;

        void Destroy() override final;

        void GetNativeWindowHandle(NativeWindowHandle* pHandle) const override final;

    private:
        void Create(const WindowDesc* pDesc, XVisualInfo* pVisualInfo);

        void ConvertXEvent(XEvent* pXEvent, Event* pEvent) const;

        void CheckEvents() const;

        void SetClassHints();

        void SetStyle();

    private:
        ::Display* m_pDisplay;
        ::Window m_XWindow;
        ::Cursor m_XCursor;
        ::Pixmap m_XIcon;
        ::Pixmap m_XIconMask;

        mutable std::queue<Event> m_Events;

        mutable WindowDesc m_Desc;
        
        mutable bool m_IsMapped;
    };
}

#endif