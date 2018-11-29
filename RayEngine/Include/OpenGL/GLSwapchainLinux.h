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

#include "GLSwapchain.h"

#if defined(RE_PLATFORM_LINUX)
#include "GLDeviceLinux.h"

namespace RayEngine
{
    namespace Graphics
    {
        class GLSwapchainLinux final : public GLSwapchain
        {
            RE_IMPLEMENT_INTERFACE(GLSwapchainLinux);

        public:
            GLSwapchainLinux(const SwapchainDesc* pDesc, GLDeviceLinux* pDevice);
            ~GLSwapchainLinux();

            void MakeCurrent() const override final;

            void Present() const override final;

        private:
            const GLDeviceLinux* m_pDevice;
            ::Display* m_pDisplay;
            
            ::Window m_XWindow;
        };
    }   
}

#endif