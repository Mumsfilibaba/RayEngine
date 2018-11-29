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
#include "GLDevice.h"

#if defined(RE_PLATFORM_LINUX)
#include <X11/Xlib.h>

namespace RayEngine
{
	namespace Graphics
	{
        class GLDeviceLinux final : public GLDevice
        {
			RE_IMPLEMENT_INTERFACE(GLDeviceLinux);

        public:
            GLDeviceLinux(const DeviceDesc* pDesc);
            GLDeviceLinux(const DeviceDesc* pDesc, ::Window xWindow, GLXFBConfig* pFBConfig);
            ~GLDeviceLinux();

            inline ::Display* GetXDisplay() const
            {
                return m_pDisplay;
            }

            inline ::Window GetXWindow() const
            {
                return m_XWindow;
            }

        private:
            void Create();

            void Create(::Window xWindow, GLXFBConfig* pFBConfig);

        private:
            ::Display* m_pDisplay;
            ::Window m_XWindow;

            bool m_IsWindowOwner;
        };
    }
}

#endif