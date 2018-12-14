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

#include <RayEngine.h>

#if defined(RE_PLATFORM_WINDOWS)
#include <System/Application.h>
#include <Win32/Win32WindowImpl.h>
#include <DX12/DX12Device.h>
#include <DX12/DX12Swapchain.h>
#include <DX11/DX11Device.h>
#include <DX11/DX11Swapchain.h>
#include <OpenGL/GLDeviceWin32.h>
#include <OpenGL/GLSwapchainWin32.h>

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Application::InitGraphics(IWindow** ppWindow, WindowDesc& windowDesc, Graphics::IDevice** ppDevice,
		Graphics::DeviceDesc& deviceDesc, GRAPHICS_API api)
	{
		Win32WindowImpl* pWindow = new Win32WindowImpl(&windowDesc);
		if (api == GRAPHICS_API_D3D12)
			*ppDevice = new Graphics::DX12Device(&deviceDesc, pWindow->GetHWND());
		else
			LOG_ERROR("Api not suppported.");

		*ppWindow = pWindow;
	}
}

#endif