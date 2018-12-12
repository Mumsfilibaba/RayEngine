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
#include "System/Application.h"

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Application::Application()
		: m_pWindow(nullptr),
		m_pDevice(nullptr),
		m_pSwapChain(nullptr)
	{
		Initialize();
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Application::~Application()
	{
		ReRelease_S(m_pContext);
		ReRelease_S(m_pSwapChain);
		ReRelease_S(m_pDevice);

		if (m_pWindow != nullptr)
		{
			m_pWindow->Destroy();
			m_pWindow = nullptr;
		}

		LOG_INFO("Exiting RayEngine");
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Application::OnUpdate()
	{
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Application::OnRender()
	{
		float color[] = { 0.392f, 0.584f, 0.929f, 1.0f };
		m_pContext->ClearRendertargetView(nullptr, color);
		m_pContext->ClearDepthStencilView(nullptr, 1.0f, 0);

		m_pSwapChain->Present();
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int32 Application::Run()
	{
		LOG_INFO("Starting RayEngine");

		m_pDevice->GetImmediateContext(&m_pContext);
		m_pContext->SetSwapChain(m_pSwapChain);

		m_pWindow->Show();
		Event event = {};
		while (event.Type != EVENT_TYPE_CLOSE)
		{
			if (m_pWindow->PeekEvent(&event))
			{
			}

			OnUpdate();
			OnRender();
		}

		return 0;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Application::Initialize()
	{
		using namespace Graphics;

		LOG_INFO("Initializing RayEngine");

		WindowDesc wnd = {};
		wnd.Style = WINDOW_STYLE_STANDARD;
		wnd.Width = 800;
		wnd.Height = 600;
		wnd.pTitle = "RayEngine";
		wnd.BackgroundColor.r = 127;
		wnd.BackgroundColor.g = 127;
		wnd.BackgroundColor.b = 127;
		wnd.x = 0;
		wnd.y = 0;
		wnd.pIcon = nullptr;
		wnd.Cursor.pImage = nullptr;

		DeviceDesc dev = {};
		dev.DeviceFlags = DEVICE_FLAG_DEBUG;
		dev.SamplerDescriptorCount = 8;
		dev.ResourceDescriptorCount = 8;
		dev.RendertargetDescriptorCount = 4;
		dev.DepthStencilDescriptorCount = 4;

		SwapchainDesc swc = {};
		swc.Width = wnd.Width;
		swc.Height = wnd.Height;
		swc.SampleCount = 1;
		swc.BackBuffer.Count = 2;
		swc.BackBuffer.Format = FORMAT_R8G8B8A8_UNORM;
		swc.DepthStencil.Format = FORMAT_D24_UNORM_S8_UINT;

		InitGraphics(&m_pWindow, wnd, &m_pDevice, dev, &m_pSwapChain, swc, GRAPHICS_API_D3D12);
	}
}