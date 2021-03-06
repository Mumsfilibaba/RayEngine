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
#include <System/System.h>
#include <DX11/DX11Device.h>
#include <DX11/DX11Swapchain.h>
#include <DX12/DX12Device.h>
#include <DX12/DX12Swapchain.h>
#include <OpenGL/GLDeviceWin32.h>
#include <OpenGL/GLSwapchain.h>
#include <Win32/Win32WindowImpl.h>

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void QuerySystemDesc(SystemDesc* pDesc)
	{
		pDesc->Platform = PLATFORM_WIN32;
		
		pDesc->ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		pDesc->ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void SendQuitMessage(int32 exitCode)
	{
		PostQuitMessage(exitCode);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CreateSystemWindow(IWindow** ppWindow, WindowDesc* pDesc)
	{

	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CreateDevice(Graphics::IDevice** ppDevice, Graphics::DeviceDesc* pDesc, GRAPHICS_API api)
	{

	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CreateWindowForRendering(IWindow** ppWindow, const WindowDesc* pWindowDesc,
		Graphics::IDevice** ppDevice, Graphics::DeviceDesc* pDeviceDesc,
		Graphics::ISwapchain** ppSwapchain, Graphics::SwapchainDesc* pSwapchainDesc,
		GRAPHICS_API api)
	{

	}
}

#endif