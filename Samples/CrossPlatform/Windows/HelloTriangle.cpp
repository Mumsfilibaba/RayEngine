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

int main(int args, char* argsv[])
{
	using namespace RayEngine;
	using namespace System;
	using namespace Math;
	using namespace Graphics;

	Log log;

	SystemInfo info = {};
	QuerySystemInfo(info);

	Bitmap icon(RE_T("Walter.png"), RE_T(""), 32, 32);
	Bitmap cursor(RE_T("Cursor.png"), RE_T(""), 24, 24);

	//Create a window
	WindowInfo windowInfo = {};
	windowInfo.BackgroundColor.r = 0;
	windowInfo.BackgroundColor.g = 0;
	windowInfo.BackgroundColor.b = 0;
	windowInfo.Flags = WINDOW_FLAG_APP_FULLSCREEN | WINDOW_FLAG_APP_NO_SLEEP;
	windowInfo.Style = WINDOWSTYLE_STANDARD_WINDOW;
#if defined(RE_PLATFORM_ANDROID)
	windowInfo.Width = info.ScreenWidth;
	windowInfo.Height = info.ScreenHeight;
#else
	windowInfo.Width = 1280;
	windowInfo.Height = 720;
	windowInfo.Cursor = cursor;
	windowInfo.Icon = icon;
#endif

	Window window(windowInfo);
	window.SetBackground(Color::CORNFLOWERBLUE);


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
			if (adapterList[i].VendorID != AdapterInfo::INTELVendorID)
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
	AdapterInfo adapterInfo = adapterList[adapterIndex];

	DeviceInfo deviceInfo = {};
	deviceInfo.Name = "Main Device";
	deviceInfo.pAdapter = &adapterInfo;
	deviceInfo.DepthStencilDescriptorCount = 4;
	deviceInfo.RendertargetDescriptorCount = 4;
	deviceInfo.ResourceDescriptorCount = 16;
	deviceInfo.SamplerDescriptorCount = 16;

	SwapchainInfo swapchainInfo = {}; 
	swapchainInfo.Name = "SwapChain";
	swapchainInfo.WindowHandle = window.GetNativeHandle();
	swapchainInfo.Width = window.GetWidth();
	swapchainInfo.Height = window.GetHeight();
	swapchainInfo.BackBuffer.Count = bufferCount;
	swapchainInfo.BackBuffer.Format = FORMAT_B8G8R8A8_UNORM;
	swapchainInfo.DepthStencil.Format = FORMAT_UNKNOWN;
	swapchainInfo.Samples = MSAA_SAMPLE_COUNT_8;

	IDevice* pDevice = nullptr;
	ISwapchain* pSwapchain = nullptr;
	pFactory->CreateDeviceAndSwapchain(&pDevice, deviceInfo, &pSwapchain, swapchainInfo);


	//Create shaders
	ShaderInfo shaderInfo = {};
	shaderInfo.Name = "VertexShader";
#if defined(RE_DEBUG)
	shaderInfo.Flags = SHADER_FLAGS_DEBUG;
#else
	shaderInfo.Flags = SHADER_FLAGS_NONE;
#endif
	shaderInfo.Source = "vs.hlsl";
	shaderInfo.FilePath = "Shaders/";
	shaderInfo.EntryPoint = "main";
	shaderInfo.SrcLang = SHADER_SOURCE_LANG_HLSL;
	shaderInfo.Type = SHADER_TYPE_VERTEX;
	
	IShader* pVs = nullptr;
	pDevice->CreateShader(&pVs, shaderInfo);
	
	shaderInfo.Name = "PixelShader";
	shaderInfo.Source = "ps.hlsl";
	shaderInfo.Type = SHADER_TYPE_PIXEL;
	
	IShader* pPs = nullptr;
	pDevice->CreateShader(&pPs, shaderInfo);
	
	
	
	//Create a RootLayout
	RootLayoutInfo rootLayoutInfo = {};
	rootLayoutInfo.Name = "RootLayout";
	rootLayoutInfo.Flags = ROOT_LAYOUT_FLAG_NONE;
	rootLayoutInfo.PipelineType = PIPELINE_TYPE_GRAPHICS;
	rootLayoutInfo.pStaticSamplers = nullptr;
	rootLayoutInfo.SamplerCount = 0;
	rootLayoutInfo.pVariables = nullptr;
	rootLayoutInfo.VariableCount = 0;
	
	IRootLayout* pRootLayout = nullptr;
	pDevice->CreateRootLayout(&pRootLayout, rootLayoutInfo);
	
	
	//Create a defenition for an inputelement (Define a vertex)
	InputElementInfo elementinfo = 
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
	PipelineStateInfo pipelinestateInfo = {};
	pipelinestateInfo.Name = "PipelineState";
	pipelinestateInfo.Type = PIPELINE_TYPE_GRAPHICS;
	pipelinestateInfo.pRootLayout = pRootLayout;
	pipelinestateInfo.GraphicsPipeline.pVertexShader = pVs;
	pipelinestateInfo.GraphicsPipeline.pPixelShader = pPs;
	pipelinestateInfo.GraphicsPipeline.DepthStencilFormat = swapchainInfo.DepthStencil.Format;
	pipelinestateInfo.GraphicsPipeline.RenderTargetCount = 1;
	pipelinestateInfo.GraphicsPipeline.RenderTargetFormats[0] = swapchainInfo.BackBuffer.Format;
	pipelinestateInfo.GraphicsPipeline.SampleCount = MSAA_SAMPLE_COUNT_1;
	pipelinestateInfo.GraphicsPipeline.SampleMask = -1;
	pipelinestateInfo.GraphicsPipeline.StripCutEnable = false;
	pipelinestateInfo.GraphicsPipeline.Topology = PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	pipelinestateInfo.GraphicsPipeline.InputLayout.ElementCount = 1;
	pipelinestateInfo.GraphicsPipeline.InputLayout.pElements = &elementinfo;
	
	pipelinestateInfo.GraphicsPipeline.DepthStencilState.DepthEnable = false;

	pipelinestateInfo.GraphicsPipeline.RasterizerState.CullMode = CULL_MODE_NONE;

	IPipelineState* pPipelineState = nullptr;
	pDevice->CreatePipelineState(&pPipelineState, pipelinestateInfo);


	
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
	
	BufferInfo vertexBufferInfo = {};
	vertexBufferInfo.Name = "VertexBuffer";
	vertexBufferInfo.Count = 3;
	vertexBufferInfo.ByteStride = sizeof(Vector3);
	vertexBufferInfo.Type = BUFFER_USAGE_VERTEX;
	vertexBufferInfo.Usage = RESOURCE_USAGE_DEFAULT;
	
	IBuffer* pVertexBuffer = nullptr;
	pDevice->CreateBuffer(&pVertexBuffer, &vbData, vertexBufferInfo);


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
			else if (event.Type == EVENT_TYPE_TOUCHPRESSED)
			{
				if (event.Touch.Position.x < (info.ScreenWidth / 2))
					Keyboard::HideVirtualKeyboardVisible();
				else
					Keyboard::ShowVirtualKeyboardVisible();
			}
			else if (event.Type == EVENT_TYPE_FOCUSCHANGED)
			{
				static ColorF oldColor = bgColor;
				if (event.FocusChanged.HasFocus)
				{
					bgColor = oldColor;
				}
				else
				{
					oldColor = bgColor;
					bgColor = ColorF::RED;
				}
			}
			else if (event.Type == EVENT_TYPE_KEYPRESSED)
			{
				if (event.Key.KeyCode == KEY_0)
					bgColor = ColorF::CORNFLOWERBLUE;
				else if (event.Key.KeyCode == KEY_1)
					bgColor = ColorF::SOFTRED;
				else if (event.Key.KeyCode == KEY_2)
					bgColor = ColorF::SOFTGREEN;
				else if (event.Key.KeyCode == KEY_3)
					bgColor = ColorF::SOFTBLUE;
				else if (event.Key.KeyCode == KEY_4)
					bgColor = ColorF::SOFTYELLOW;
				else if (event.Key.KeyCode == KEY_5)
					bgColor = ColorF::SOFTGRAY;
				else if (event.Key.KeyCode == KEY_6)
					bgColor = ColorF::RED;
				else if (event.Key.KeyCode == KEY_7)
					bgColor = ColorF::GREEN;
				else if (event.Key.KeyCode == KEY_8)
					bgColor = ColorF::BLUE;
				else if (event.Key.KeyCode == KEY_9)
					bgColor = ColorF::WARMWHITE;
			}
			else if (event.Type == EVENT_TYPE_SENSORCHANGED)
			{
				if (event.Sensor.Type == SENSOR_TYPE_ACCELEROMETER)
				{
					strength = (event.Sensor.Data.Accelerometer.x + 11.0f) / 22.0f;
					lastAccelerometer = event.Sensor.Data.Accelerometer;
	
					if (strength > 1.0f)
						strength = 1.0f;
					else if (strength < 0.0f)
						strength = 0.0f;
				}
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