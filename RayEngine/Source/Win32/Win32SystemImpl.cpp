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
#include "../../Include//Utilities/EngineUtilities.h"
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
	void CreateOpenGLDevice(HWND hwnd, Graphics::IDevice** ppDevice, Graphics::DeviceDesc* pDeviceDesc,
		Graphics::ISwapchain** ppSwapchain, Graphics::SwapchainDesc* pSwapchainDesc)
	{
		int32 depthBits = FormatDepthBits(pSwapchainDesc->DepthStencil.Format);
		int32 stencilBits = FormatStencilBits(pSwapchainDesc->DepthStencil.Format);
		int32 colorSpace = FormatIsSRGB(pSwapchainDesc->BackBuffer.Format) ? WGL_COLORSPACE_SRGB_EXT : WGL_COLORSPACE_LINEAR_EXT;
		int32 redBits = FormatRedComponentBits(pSwapchainDesc->BackBuffer.Format);
		int32 greenBits = FormatGreenComponentBits(pSwapchainDesc->BackBuffer.Format);
		int32 blueBits = FormatBlueComponentBits(pSwapchainDesc->BackBuffer.Format);
		int32 alphaBits = FormatAlphaComponentBits(pSwapchainDesc->BackBuffer.Format);

		int32 pixelFormatAttribs[] =
		{
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
			WGL_SWAP_METHOD_ARB, WGL_SWAP_COPY_ARB,
			WGL_STEREO_ARB, GL_FALSE,
			WGL_COLORSPACE_EXT, colorSpace,
			WGL_SAMPLE_BUFFERS_ARB, 1,
			WGL_SAMPLES_ARB, 1,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_RED_BITS_ARB, redBits,
			WGL_GREEN_BITS_ARB, greenBits,
			WGL_BLUE_BITS_ARB, blueBits,
			WGL_ALPHA_BITS_ARB, alphaBits,
			depthBits > 0 ? WGL_DEPTH_BITS_ARB : 0 , depthBits,
			stencilBits > 0 ? WGL_STENCIL_BITS_ARB : 0, stencilBits,
			0, 0
		};

		*ppDevice = new Graphics::GLDeviceWin32(pDeviceDesc, hwnd, pixelFormatAttribs);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void CreateWindowForRendering(IWindow** ppWindow, const WindowDesc* pWindowDesc,
		Graphics::IDevice** ppDevice, Graphics::DeviceDesc* pDeviceDesc,
		Graphics::ISwapchain** ppSwapchain, Graphics::SwapchainDesc* pSwapchainDesc,
		GRAPHICS_API api)
	{
		Win32WindowImpl* pWindow = new Win32WindowImpl(pWindowDesc);
		if (api == GRAPHICS_API_D3D12)
		{
		}
		else if (api == GRAPHICS_API_D3D11)
		{
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