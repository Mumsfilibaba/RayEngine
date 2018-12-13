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
#include <System/Application.h>
#include <Graphics/IShader.h>
#include <Graphics/IPipelineState.h>
#include <Graphics/Viewport.h>

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Application::Application(GRAPHICS_API api)
		: m_pWindow(nullptr),
		m_pDevice(nullptr),
		m_pSwapChain(nullptr),
		m_pPipeline(nullptr),
		m_Api(api)
	{
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Application::~Application()
	{
		ReRelease_S(m_pPipeline);
		ReRelease_S(m_pContext);
		ReRelease_S(m_pSwapChain);
		ReRelease_S(m_pDevice);

		if (m_pWindow != nullptr)
		{
			m_pWindow->Destroy();
			m_pWindow = nullptr;
		}

		LOG_INFO("Destroying Application");
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Application::OnUpdate()
	{
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Application::OnRender()
	{
		using namespace Graphics;

		float color[] = { 0.392f, 0.584f, 0.929f, 1.0f };
		GetContext()->ClearRendertargetView(nullptr, color);
		GetContext()->ClearDepthStencilView(nullptr, 1.0f, 0);

		Viewport viewport = {};
		viewport.Width = static_cast<float>(m_pWindow->GetWidth());
		viewport.Height = static_cast<float>(m_pWindow->GetHeight());
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		Rect rect = {};
		rect.Top = 0;
		rect.Bottom = m_pWindow->GetHeight();
		rect.Left = 0;
		rect.Right = m_pWindow->GetWidth();

		GetContext()->SetScissorRects(&rect);
		GetContext()->SetViewports(&viewport);

		GetContext()->SetPrimitiveTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		GetContext()->SetPipelineState(m_pPipeline);

		GetContext()->Draw(0, 3);

		GetSwapChain()->Present();
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int32 Application::Run()
	{
		OnCreate();

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

		LOG_INFO("Exiting RayEngine");
		return 0;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Application::OnCreate()
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

		InitGraphics(&m_pWindow, wnd, &m_pDevice, dev, &m_pSwapChain, swc, m_Api);

		std::string vs = 
			"struct VS_OUT\n"
			"{\n"
			"	float4 pos : SV_Position;\n"
			"};\n"
			"\n"
			"VS_OUT main(uint id : SV_VertexID)\n"
			"{\n"
			"	float2 coords = float2((id << 1) & 2, id & 2);\n"
			"	VS_OUT output;\n"
			"	if (id == 0) { output.pos = float4(0.0f, 0.5f, 0.0f, 1.0f); }\n"
			"	else if (id == 1) { output.pos = float4(0.5f, -0.5f, 0.0f, 1.0f); }\n"
			"	else if (id == 2) { output.pos = float4(-0.5f, -0.5f, 0.0f, 1.0f); }\n"
			"	else { output.pos = float4(0.0f, 0.0f, 0.0f, 1.0f); }\n"
			"	return output;\n"
			"}\n";

		std::string ps =
			"struct PS_IN\n"
			"{\n"
			"	float4 pos : SV_Position;\n"
			"};\n"
			"\n"
			"float4 main(PS_IN input) : SV_Target0\n"
			"{\n"
			"	return float4(1.0f, 1.0f, 1.0f, 1.0f);\n"
			"}\n";

		ShaderDesc shader = {};
		shader.Flags = SHADER_FLAGS_DEBUG;
		shader.pEntryPoint = "main";
		shader.pSource = vs.c_str();
		shader.Type = SHADER_TYPE_VERTEX;
		shader.SrcLang = SHADER_SOURCE_LANG_HLSL;
		
		IShader* pVS = nullptr;
		m_pDevice->CreateShader(&pVS, &shader);

		shader.pSource = ps.c_str();
		shader.Type = SHADER_TYPE_PIXEL;
		
		IShader* pPS = nullptr;
		m_pDevice->CreateShader(&pPS, &shader);

		PipelineStateDesc pipeline = PipelineStateDesc::DefaultGraphicsPipeline();
		pipeline.Type = PIPELINE_TYPE_GRAPHICS;
		pipeline.Graphics.pVertexShader = pVS;
		pipeline.Graphics.pPixelShader = pPS;

		pipeline.Graphics.RenderTargetCount = 1;
		pipeline.Graphics.RenderTargetFormats[0] = swc.BackBuffer.Format;
		pipeline.Graphics.DepthStencilFormat = swc.DepthStencil.Format;
		pipeline.Graphics.SampleCount = swc.SampleCount;
		
		pipeline.Graphics.Topology = PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		pipeline.Graphics.InputLayout.ElementCount = 0;
		pipeline.Graphics.InputLayout.pElements = nullptr;

		pipeline.Graphics.DepthStencilState.DepthEnable = true;

		m_pDevice->CreatePipelineState(&m_pPipeline, &pipeline);

		ReRelease_S(pVS);
		ReRelease_S(pPS);
	}
}