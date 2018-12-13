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
#include "../../Include/OpenGL/GLSwapchainWin32.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLSwapchainWin32::GLSwapchainWin32(const SwapchainDesc* pDesc, GLDeviceWin32* pDevice)
			: GLSwapchain(pDevice, pDesc),
			m_pDevice(nullptr),
			m_HDC(0)
		{
			m_pDevice = pDevice;
			m_HDC = pDevice->GetHDC();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLSwapchainWin32::~GLSwapchainWin32()
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLSwapchainWin32::MakeCurrent() const
		{
			wglMakeCurrent(m_HDC, GetCurrentContext());
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLSwapchainWin32::Present() const
		{
			SwapBuffers(m_HDC);
		}
	}
}

#endif