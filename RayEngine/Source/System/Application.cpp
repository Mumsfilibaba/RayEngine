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
#include <Graphics/IRenderer.h>
#include <Graphics/Viewport.h>

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Application::Application(GRAPHICS_API api)
		: m_pWindow(nullptr),
		m_pDevice(nullptr),
		m_pRenderer(nullptr),
		m_pPipeline(nullptr),
		m_Api(api)
	{
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Application::~Application()
	{
		ReRelease_S(m_pPipeline);
		ReRelease_S(m_pRenderer);
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

		m_pRenderer->Begin();

		float color[] = { 0.392f, 0.584f, 0.929f, 1.0f };
		m_pRenderer->Clear(color);
		m_pRenderer->Draw();

		m_pRenderer->End();

		m_pRenderer->Present();
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int32 Application::Run()
	{
		OnCreate();

		LOG_INFO("Starting RayEngine");

		m_pWindow->Show();
		Event event = {};
		while (event.Type != EVENT_TYPE_CLOSE)
		{
			if (m_pWindow->PeekEvent(&event))
			{
				if (event.Type == EVENT_TYPE_RESIZE)
				{
				}
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
		wnd.Width = 1024;
		wnd.Height = 768;
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
		dev.SampleCount = 1;
		dev.BackBuffer.Count = 2;
		dev.BackBuffer.Format = FORMAT_R8G8B8A8_UNORM;
		dev.DepthStencil.Format = FORMAT_D24_UNORM_S8_UINT;
		dev.Width = wnd.Width;
		dev.Height = wnd.Height;

		InitGraphics(&m_pWindow, wnd, &m_pDevice, dev, m_Api);
		m_pRenderer = m_pDevice->CreateRenderer();

		std::string vs;
		std::string ps;

		ShaderDesc shader = {};
		shader.Flags = SHADER_FLAGS_DEBUG;
		shader.pEntryPoint = "main";
		shader.Type = SHADER_TYPE_VERTEX;
		
		if (m_Api == GRAPHICS_API_OPENGL)
		{
			shader.SrcLang = SHADER_SOURCE_LANG_GLSL;
			vs =
				"#version 330\n"
				"\n"
				"void main()\n"
				"{\n"
				"	vec4 pos = vec4(0.0);\n"
				"	if (gl_VertexID == 0) { pos = vec4(0.0f, 0.5f, 0.0f, 1.0f); }\n"
				"	else if (gl_VertexID == 1) { pos = vec4(0.5f, -0.5f, 0.0f, 1.0f); }\n"
				"	else if (gl_VertexID == 2) { pos = vec4(-0.5f, -0.5f, 0.0f, 1.0f); }\n"
				"	else { pos = vec4(0.0f, 0.0f, 0.0f, 1.0f); }\n"
				"	gl_Position = pos;\n"
				"}\n";
			ps =
				"#version 330\n"
				"\n"
				"void main()\n"
				"{\n"
				"	gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
				"}\n";
		}
		else
		{
			shader.SrcLang = SHADER_SOURCE_LANG_HLSL;
			vs =
				"struct VS_OUT\n"
				"{\n"
				"	float4 pos : SV_Position;\n"
				"};\n"
				"\n"
				"VS_OUT main(uint id : SV_VertexID)\n"
				"{\n"
				"	VS_OUT output;\n"
				"	if (id == 0) { output.pos = float4(0.0f, 0.5f, 0.0f, 1.0f); }\n"
				"	else if (id == 1) { output.pos = float4(0.5f, -0.5f, 0.0f, 1.0f); }\n"
				"	else if (id == 2) { output.pos = float4(-0.5f, -0.5f, 0.0f, 1.0f); }\n"
				"	else { output.pos = float4(0.0f, 0.0f, 0.0f, 1.0f); }\n"
				"	return output;\n"
				"}\n";	
			ps =
				"struct PS_IN\n"
				"{\n"
				"	float4 pos : SV_Position;\n"
				"};\n"
				"\n"
				"float4 main(PS_IN input) : SV_Target0\n"
				"{\n"
				"	return float4(1.0f, 1.0f, 1.0f, 1.0f);\n"
				"}\n";
		}

		shader.pSource = vs.c_str();
		
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
		pipeline.Graphics.RenderTargetFormats[0] = dev.BackBuffer.Format;
		pipeline.Graphics.DepthStencilFormat = dev.DepthStencil.Format;
		pipeline.Graphics.SampleCount = dev.SampleCount;
		
		pipeline.Graphics.Topology = PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		pipeline.Graphics.InputLayout.ElementCount = 0;
		pipeline.Graphics.InputLayout.pElements = nullptr;

		pipeline.Graphics.DepthStencilState.DepthEnable = true;

		m_pDevice->CreatePipelineState(&m_pPipeline, &pipeline);

		ReRelease_S(pVS);
		ReRelease_S(pPS);
	}
}