#include <System/Window.h>
#include <System/Clock.h>
#include <System/Log.h>
#include <System/System.h>
#include <System/Bitmap.h>
#include <System/Mouse.h>
#include <System/Keyboard.h>
#include <System/Clipboard.h>
#include <System/TouchScreen.h>
#include <System/Sensors.h>

#include <Graphics/IFactory.h>
#include <Graphics/IDevice.h>
#include <Graphics/IDeviceContext.h>
#include <Graphics/ISwapchain.h>
#include <Graphics/IRenderTargetView.h>
#include <Graphics/IShader.h>
#include <Graphics/IRootLayout.h>
#include <Graphics/IPipelineState.h>
#include <Graphics/ITexture.h>
#include <Graphics/IDepthStencilView.h>
#include <Graphics/IBuffer.h>
#include <Graphics/Viewport.h>

#include <Math/RandomGenerator.h>

#include <Utilities/EngineUtilities.h>

int main(int args, char* argsv[])
{
	using namespace RayEngine;
	using namespace System;
	using namespace Math;
	using namespace Graphics;

	Log log;

	SystemDesc systemDesc = {};
	QuerySystemDesc(&systemDesc);

	Bitmap icon(RE_T("Walter.png"), RE_T(""), 32, 32);
	Bitmap cursor(RE_T("Cursor.png"), RE_T(""), 24, 24);

	//Create a window
	WindowDesc windowDesc = {};
	windowDesc.BackgroundColor.r = 0;
	windowDesc.BackgroundColor.g = 0;
	windowDesc.BackgroundColor.b = 0;
	windowDesc.Flags = WINDOW_FLAG_APP_FULLSCREEN | WINDOW_FLAG_APP_NO_SLEEP;
	windowDesc.Style = WINDOWSTYLE_STANDARD_WINDOW;
#if defined(RE_PLATFORM_ANDROID)
	windowDesc.Width = info.ScreenWidth;
	windowDesc.Height = info.ScreenHeight;
#else
	windowDesc.Width = 1280;
	windowDesc.Height = 720;
	windowDesc.Cursor = cursor;
	windowDesc.Icon = icon;
#endif

	Window window(windowDesc);


	//Create a factory for the api
	IFactory* pFactory = IFactory::Create(GRAPHICS_API_OPENGL, true);
	pFactory->SetName("Factory");

	//Retrive all adapters on the system that the api can find
	int32 adapterIndex = 0;
	int32 adapterCount = 0;
	AdapterList adapterList = {};
	pFactory->EnumerateAdapters(adapterList);

	if (adapterList.Count > 1)
	{
		for (int32 i = 0; i < adapterList.Count; i++)
		{
			//Do not choose an intel integrated one if there are more than 1 adapter
			if (adapterList[i].VendorID != AdapterDesc::INTELVendorID)
			{
				adapterIndex = i;
				break;
			}
		}
	}


	//Print the chosen adapter
	log.Write(LOG_SEVERITY_INFO, adapterList[adapterIndex].VendorName);
	log.Write(LOG_SEVERITY_INFO, adapterList[adapterIndex].ModelName);


	//Create a device and swapchain
	constexpr int32 bufferCount = 2;
	AdapterDesc adapterDesc = adapterList[adapterIndex];

	DeviceDesc deviceDesc = {};
	deviceDesc.Name = "Main Device";
	deviceDesc.pAdapter = &adapterDesc;
	deviceDesc.DepthStencilDescriptorCount = 4;
	deviceDesc.RendertargetDescriptorCount = 4;
	deviceDesc.ResourceDescriptorCount = 16;
	deviceDesc.SamplerDescriptorCount = 16;

	SwapchainDesc swapchainDesc = {}; 
	swapchainDesc.Name = "SwapChain";
	swapchainDesc.WindowHandle = window.GetNativeHandle();
	swapchainDesc.Width = window.GetWidth();
	swapchainDesc.Height = window.GetHeight();
	swapchainDesc.BackBuffer.Count = bufferCount;
	swapchainDesc.BackBuffer.Format = FORMAT_B8G8R8A8_UNORM;
	swapchainDesc.DepthStencil.Format = FORMAT_UNKNOWN;
	swapchainDesc.Samples = MSAA_SAMPLE_COUNT_1;

	IDevice* pDevice = nullptr;
	ISwapchain* pSwapchain = nullptr;
	pFactory->CreateDeviceAndSwapchain(&pDevice, &deviceDesc, &pSwapchain, &swapchainDesc);


	//Create shaders
	ShaderDesc shaderDesc = {};
	shaderDesc.Name = "VertexShader";
#if defined(RE_DEBUG)
	shaderDesc.Flags = SHADER_FLAGS_DEBUG;
#else
	shaderDesc.Flags = SHADER_FLAGS_NONE;
#endif
	shaderDesc.Type = SHADER_TYPE_VERTEX;
	shaderDesc.EntryPoint = "main";

	std::string source;
	if (pFactory->GetGraphicsApi() == GRAPHICS_API_OPENGL)
	{
		shaderDesc.SrcLang = SHADER_SOURCE_LANG_GLSL;
		source = ReadFullFile("vs.glsl", "Shaders/");
	}
	else
	{
		shaderDesc.SrcLang = SHADER_SOURCE_LANG_HLSL;
		source = ReadFullFile("vs.hlsl", "Shaders/");
	}
	shaderDesc.Source = source;

	
	IShader* pVs = nullptr;
	pDevice->CreateShader(&pVs, &shaderDesc);
	
	shaderDesc.Type = SHADER_TYPE_PIXEL;
	shaderDesc.Name = "PixelShader";
	if (pFactory->GetGraphicsApi() == GRAPHICS_API_OPENGL)
	{
		source = ReadFullFile("ps.glsl", "Shaders/");
	}
	else
	{
		source = ReadFullFile("ps.hlsl", "Shaders/");
	}
	shaderDesc.Source = source;
	
	IShader* pPs = nullptr;
	pDevice->CreateShader(&pPs, &shaderDesc);
	
	
	
	//Create a RootLayout
	RootLayoutDesc rootLayoutDesc = {};
	rootLayoutDesc.Name = "RootLayout";
	rootLayoutDesc.Flags = ROOT_LAYOUT_FLAG_NONE;
	rootLayoutDesc.PipelineType = PIPELINE_TYPE_GRAPHICS;
	rootLayoutDesc.pStaticSamplers = nullptr;
	rootLayoutDesc.SamplerCount = 0;
	rootLayoutDesc.pVariables = nullptr;
	rootLayoutDesc.VariableCount = 0;
	
	IRootLayout* pRootLayout = nullptr;
	pDevice->CreateRootLayout(&pRootLayout, &rootLayoutDesc);
	
	
	//Create a defenition for an inputelement (Define a vertex)
	InputElementDesc elementDesc = 
	{ 
		"POSITION",
		0,
		FORMAT_R32G32B32_FLOAT,
		ELEMENT_STEP_TYPE_VERTEX,
		0,
		0,
		0,
		sizeof(Math::Vector3)
	};


	//Create a pipelinestate
	PipelineStateDesc pipelinestateDesc = {};
	pipelinestateDesc.Name = "PipelineState";
	pipelinestateDesc.Type = PIPELINE_TYPE_GRAPHICS;
	pipelinestateDesc.pRootLayout = pRootLayout;
	pipelinestateDesc.GraphicsPipeline.pVertexShader = pVs;
	pipelinestateDesc.GraphicsPipeline.pPixelShader = pPs;
	pipelinestateDesc.GraphicsPipeline.DepthStencilFormat = swapchainDesc.DepthStencil.Format;
	pipelinestateDesc.GraphicsPipeline.RenderTargetCount = 1;
	pipelinestateDesc.GraphicsPipeline.RenderTargetFormats[0] = swapchainDesc.BackBuffer.Format;
	pipelinestateDesc.GraphicsPipeline.SampleCount = MSAA_SAMPLE_COUNT_1;
	pipelinestateDesc.GraphicsPipeline.SampleMask = -1;
	pipelinestateDesc.GraphicsPipeline.Topology = PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	pipelinestateDesc.GraphicsPipeline.InputLayout.ElementCount = 1;
	pipelinestateDesc.GraphicsPipeline.InputLayout.pElements = &elementDesc;
	
	pipelinestateDesc.GraphicsPipeline.DepthStencilState.DepthEnable = false;
	pipelinestateDesc.GraphicsPipeline.DepthStencilState.StencilEnable = false;
	pipelinestateDesc.GraphicsPipeline.DepthStencilState.BackFace = pipelinestateDesc.GraphicsPipeline.DepthStencilState.FrontFace;

	pipelinestateDesc.GraphicsPipeline.RasterizerState.AntialiasedLineEnable = false;
	pipelinestateDesc.GraphicsPipeline.RasterizerState.ConservativeRasterizerEnable = false;
	pipelinestateDesc.GraphicsPipeline.RasterizerState.FillMode = FILL_MODE_SOLID;
	pipelinestateDesc.GraphicsPipeline.RasterizerState.MultisampleEnable = false;
	pipelinestateDesc.GraphicsPipeline.RasterizerState.DepthClipEnable = false;
	pipelinestateDesc.GraphicsPipeline.RasterizerState.CullMode = CULL_MODE_NONE;

	pipelinestateDesc.GraphicsPipeline.BlendState.IndependentBlendEnable = false;
	pipelinestateDesc.GraphicsPipeline.BlendState.AlphaToCoverageEnable = false;
	pipelinestateDesc.GraphicsPipeline.BlendState.LogicOpEnable = false;
	pipelinestateDesc.GraphicsPipeline.BlendState.RenderTargets[0].WriteMask = COLOR_WRITE_ENABLE_ALL;

	IPipelineState* pPipelineState = nullptr;
	pDevice->CreatePipelineState(&pPipelineState, &pipelinestateDesc);


	
	//Create a vertexbuffer
	Vector3 vertices[3] = 
	{
		Vector3(0.0f, 0.5f, 0.0f),
		Vector3(0.5f, -0.5f, 0.0f),
		Vector3(-0.5f, -0.5f, 0.0f),
	};
	
	ResourceData vbData = {};
	vbData.pData = vertices;
	vbData.WidthOrCount = 3;
	vbData.ByteStride = sizeof(Vector3);
	vbData.Height = 1;
	
	BufferDesc vertexBufferDesc = {};
	vertexBufferDesc.Name = "VertexBuffer";
	vertexBufferDesc.Count = 3;
	vertexBufferDesc.ByteStride = sizeof(Vector3);
	vertexBufferDesc.Type = BUFFER_USAGE_VERTEX;
	vertexBufferDesc.Usage = RESOURCE_USAGE_DEFAULT;
	
	IBuffer* pVertexBuffer = nullptr;
	pDevice->CreateBuffer(&pVertexBuffer, &vbData, &vertexBufferDesc);


	//Get devicecontext
	IDeviceContext* pDeviceContext = nullptr;
	pDevice->GetImmediateContext(&pDeviceContext);


	//Set the default framebuffer
	pDeviceContext->SetSwapChain(pSwapchain);


	//Show window when all initialization is completed
	window.Show();


	Clock clock;
	RandomGenerator ran;
	
	Math::Vector3 lastAccelerometer;

	float strength = 1.0f;
	ColorF bgColor = ColorF::CORNFLOWERBLUE;

	//Check for sensor support
	if (Sensors::SensorSupported(SENSOR_TYPE_ACCELEROMETER))
	{
		log.Write(LOG_SEVERITY_INFO, "Accelerometer supported");

		Sensors::EnableSensor(SENSOR_TYPE_ACCELEROMETER);
		
		if (!Sensors::SetRefreshRate(SENSOR_TYPE_ACCELEROMETER, TimeStamp::Seconds(0.5)))
			log.Write(LOG_SEVERITY_WARNING, "Could not set Sensor refreshrate");
	}


	Event event;
	while (event.Type != EVENT_TYPE_QUIT)
	{
		clock.Tick();
	
		while (window.PeekEvent(event))
		{
			if (event.Type == EVENT_TYPE_CLOSE)
			{
				window.SendQuitEvent(0);
			}
			else if (event.Type == EVENT_TYPE_RESIZE)
			{
				pSwapchain->Resize(event.Resize.Width, event.Resize.Height);
			}
		}
	
	
		//Get and clear currentbackbuffer and depthstencil
		ColorF backbufferColor = ColorF::CORNFLOWERBLUE;
		pDeviceContext->ClearRendertargetView(nullptr, backbufferColor);
		

		//Set the viewport
		Viewport viewport = {};
		viewport.Height = static_cast<float>(window.GetHeight());
		viewport.Width = static_cast<float>(window.GetWidth());
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
	
		pDeviceContext->SetViewports(viewport);
	
	
		//Set the scissor rect
		Math::Rectangle scissorRect = {};
		scissorRect.BottomRight.x = static_cast<float>(window.GetWidth());
		scissorRect.BottomRight.y = static_cast<float>(window.GetHeight());
		
		pDeviceContext->SetScissorRects(scissorRect);
	
		//Set PipelineState and RootLayout
		pDeviceContext->SetPipelineState(pPipelineState);
		pDeviceContext->SetRootLayout(pRootLayout);
	
		pDeviceContext->SetPrimitiveTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pDeviceContext->SetVertexBuffers(pVertexBuffer, 0);
		pDeviceContext->Draw(0, 3);
	
		pSwapchain->Present();
	}


	ReRelease_S(pDeviceContext);
	ReRelease_S(pPipelineState);
	ReRelease_S(pVs);
	ReRelease_S(pPs);
	ReRelease_S(pVertexBuffer);
	ReRelease_S(pRootLayout);
	ReRelease_S(pSwapchain);
	ReRelease_S(pDevice);
	ReRelease_S(pFactory);

	return 0;
}