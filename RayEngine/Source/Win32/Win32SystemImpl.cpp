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

#include "../../Include/System/System.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "../../Include/DX11/DX11Device.h"
#include "../../Include/DX11/DX11Swapchain.h"
#include "../../Include/OpenGL/GLDeviceWin32.h"
#include "../../Include/OpenGL/GLSwapchain.h"
#include "../../Include/Win32/Win32WindowImpl.h"

#if defined(CreateWindow)
#undef CreateWindow
#endif

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
	void CreateWindow(IWindow** ppWindow, WindowDesc* pDesc)
	{
		*ppWindow = new Win32WindowImpl(pDesc);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CreateDevice(Graphics::IDevice** ppDevice, Graphics::DeviceDesc* pDesc, GRAPHICS_API api)
	{
		if (api == GRAPHICS_API_D3D12)
		{
		}
		else if (api == GRAPHICS_API_D3D11)
		{
			*ppDevice = new Graphics::DX11Device(pDesc);
		}
		else if (api == GRAPHICS_API_OPENGL)
		{
			*ppDevice = new Graphics::GLDeviceWin32(pDesc);
		}
		else if (api == GRAPHICS_API_VULKAN)
		{
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CreateWindowForRendering(IWindow** ppWindow, const WindowDesc* pWindowDesc,
		Graphics::IDevice** ppDevice, Graphics::DeviceDesc* pDeviceDesc,
		Graphics::ISwapchain** ppSwapchain, Graphics::SwapchainDesc* pSwapchainDesc,
		GRAPHICS_API api)
	{
		using namespace Graphics;

		Win32WindowImpl* pWindow = new Win32WindowImpl(pWindowDesc);
		if (api == GRAPHICS_API_D3D12)
		{
		}
		else if (api == GRAPHICS_API_D3D11)
		{
			DX11Device* pDevice = new DX11Device(pDeviceDesc);;
			*ppDevice = pDevice;

			*ppSwapchain = new Graphics::DX11Swapchain(pDevice, pSwapchainDesc);
		}
		else if (api == GRAPHICS_API_OPENGL)
		{
			CreateOpenGLDevice(pWindow->GetHWND(), ppDevice, pDeviceDesc, ppSwapchain, pSwapchainDesc);
		}
		else if (api == GRAPHICS_API_VULKAN)
		{
		}

		*ppWindow = pWindow;
	}
}

#endif