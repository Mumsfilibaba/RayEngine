#pragma once
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
#include "IWindow.h"
#include <Graphics/IDevice.h>
#include <Graphics/IDeviceContext.h>
#include <Graphics/ISwapchain.h>

namespace RayEngine
{
	class RAYENGINE_API Application
	{
		RE_UNIQUE_OBJECT(Application);

	public:
		Application(GRAPHICS_API api);
		~Application();

		int32 Run();

		virtual void OnUpdate();

		virtual void OnRender();

	protected:
		inline IWindow* GetWindow() const
		{
			return m_pWindow;
		}

		inline Graphics::ISwapchain* GetSwapChain() const
		{
			return m_pSwapChain;
		}

		inline Graphics::IDevice* GetDevice() const
		{
			return m_pDevice;
		}

		inline Graphics::IDeviceContext* GetContext() const
		{
			return m_pContext;
		}

	private:
		void Initialize(GRAPHICS_API api);

	private:
		IWindow* m_pWindow;
		Graphics::IDevice* m_pDevice;
		Graphics::IDeviceContext* m_pContext;
		Graphics::ISwapchain* m_pSwapChain;

	private:
		static void InitGraphics(IWindow** ppWindow, WindowDesc& windowDesc, Graphics::IDevice** ppDevice, Graphics::DeviceDesc& deviceDesc,
			Graphics::ISwapchain** ppSwapChain, Graphics::SwapchainDesc& swapChainDesc, GRAPHICS_API api);
	};
}