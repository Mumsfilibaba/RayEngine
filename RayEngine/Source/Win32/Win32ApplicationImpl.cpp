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

#if defined(RE_PLATFORM_WINDOWS)
#include "System/Application.h"
#include "Win32/Win32WindowImpl.h"
#include "DX12/DX12Device.h"
#include "DX12/DX12Swapchain.h"

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void InitD3D12(HWND hwnd, Graphics::IDevice** ppDevice, Graphics::DeviceDesc& deviceDesc,
		Graphics::ISwapchain** ppSwapChain, Graphics::SwapchainDesc& swapChainDesc)
	{
		using namespace Graphics;

		DX12Device* pDevice = new DX12Device(&deviceDesc);
		*ppDevice = pDevice;

		DX12Swapchain* pSwapChain = new DX12Swapchain(pDevice, &swapChainDesc, hwnd);
		*ppSwapChain = pSwapChain;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void InitD3D11(HWND hwnd, Graphics::IDevice** ppDevice, Graphics::DeviceDesc& deviceDesc,
		Graphics::ISwapchain** ppSwapChain, Graphics::SwapchainDesc& swapChainDesc)
	{
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Application::InitGraphics(IWindow** ppWindow, WindowDesc& windowDesc,
		Graphics::IDevice** ppDevice, Graphics::DeviceDesc& deviceDesc,
		Graphics::ISwapchain** ppSwapChain, Graphics::SwapchainDesc& swapChainDesc,
		GRAPHICS_API api)
	{
		Win32WindowImpl* pWindow = nullptr;
		if (api == GRAPHICS_API_D3D12)
		{
			pWindow = new Win32WindowImpl(&windowDesc);
			InitD3D12(pWindow->GetHWND(),  ppDevice, deviceDesc, ppSwapChain, swapChainDesc);
		}
		else if (api == GRAPHICS_API_D3D11)
		{

		}
		else
		{
			LOG_ERROR("No other APIs than GRAPHICS_API_D3D12 at the moment.");
		}

		*ppWindow = pWindow;
	}
}

#endif