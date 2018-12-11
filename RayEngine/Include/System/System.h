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
#include "RayEngine.h"

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class IWindow;
	struct WindowDesc;

	namespace Graphics
	{
		class IDevice;
		struct DeviceDesc;

		class ISwapchain;
		struct SwapchainDesc;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum PLATFORM
	{
		PLATFORM_UNKNOWN = 0,
		PLATFORM_ANDROID = 1,
		PLATFORM_WIN32 = 2,
		PLATFORM_LINUX = 3
	};


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct SystemDesc
	{
		PLATFORM Platform;
		int32 ScreenWidth;
		int32 ScreenHeight;
	};


	/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		Returns system information.

		pDesc - Valid pointer to a SystemDesc.

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
	void QuerySystemDesc(SystemDesc* pDesc);

	/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		Sends a message that the application is about to quit to all windows created.

		exitCode - The exit code that will be sent with the quitmessage

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
	void SendQuitMessage(int32 exitCode);

	/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		Creates a system-specific window.

		ppWindow - A valid pointer to a IWindow*

		pDesc - A valid pointer to a WindowDesc

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
	void CreateSystemWindow(IWindow** ppWindow, WindowDesc* pDesc);

	/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		Creates a system-specific renderingdevice used to interface with the graphicscard.

		ppDevice - A valid pointer to a IDevice*.

		pDesc - A valid pointer to a DeviceDesc.

		api - A enum specifying what graphics api to use. This varies from system to system

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
	void CreateDevice(Graphics::IDevice** ppDevice, Graphics::DeviceDesc* pDesc, GRAPHICS_API api);

	/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		Creates a system-specific window, renderingdevice and swapchain made for rendering to a window.

		pWindowDesc - A valid pointer to a IWindow*

		pDesc - A valid pointer to a WindowDesc structure

		ppDevice - A valid pointer to a IDevice*.

		pDesc - A valid pointer to a DeviceDesc structure.

		ppSwapchain - A valid pointer to a ISwapchain*.

		pSwapchainDesc - A valid pointer to a SwapchainDesc structure. 

		api - A enum specifying what graphics api to use. This varies from system to system

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
	void CreateWindowForRendering(	IWindow** ppWindow, const WindowDesc* pWindowDesc,
									Graphics::IDevice** ppDevice, Graphics::DeviceDesc* pDeviceDesc,
									Graphics::ISwapchain** ppSwapchain, Graphics::SwapchainDesc* pSwapchainDesc,
									GRAPHICS_API api);
}