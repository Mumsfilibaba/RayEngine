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

#include "../../Include/OpenGL/GLSwapchainLinux.h"

#if defined(RE_PLATFORM_LINUX)

namespace RayEngine
{
    namespace Graphics
    {  
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////        
        GLSwapchainLinux::GLSwapchainLinux(const SwapchainDesc* pDesc, GLDeviceLinux* pDevice)
            : GLSwapchain(pDevice, pDesc),
            m_pDevice(nullptr),
            m_pDisplay(nullptr),
            m_XWindow(0)
        {
            m_pDevice = pDevice;
            m_pDisplay = pDevice->GetXDisplay();
            m_XWindow = pDevice->GetXWindow();
        }
        

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        GLSwapchainLinux::~GLSwapchainLinux()
        {
            LOG_INFO("Destroyed Linux Swapchain");
        }


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void GLSwapchainLinux::Present() const
        {
            glXSwapBuffers(m_pDisplay, m_XWindow);
        }


        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void GLSwapchainLinux::MakeCurrent() const
        {
            glXMakeCurrent(m_pDisplay, m_XWindow, GetCurrentContext());
        }
    }   
}

#endif