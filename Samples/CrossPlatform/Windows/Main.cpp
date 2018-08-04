#include <System/Window.h>
#include <System/Clock.h>
#include <System/Log.h>
#include <System/System.h>
#include <System/Bitmap.h>
#include <System/Mouse.h>
#include <System/Keyboard.h>
#include <System/Clipboard.h>
#include <Math/RandomGenerator.h>
#include <System/TouchScreen.h>
#include <System/Sensors.h>
#include <Graphics/IFactory.h>
#include <Graphics/TextureLoader.h>

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

	Event event;
	Window window(windowInfo);

	window.SetBackground(Color::CORNFLOWERBLUE);

	IFactory* factory = IFactory::Create(GRAPHICS_API_VULKAN, true);
	
	int32 adapterIndex = 0;
	int32 adapterCount = 0;
	AdapterList adapters = {};
	factory->EnumerateAdapters(adapters);

#if defined(RE_PLATFORM_WINDOWS)

	for (int32 i = 0; i < adapters.Count; i++)
	{
		if (adapters[i].VendorID != AdapterInfo::INTELVendorID)
		{
			adapterIndex = i;
			break;
		}
	}
#endif

	log.Write(LOG_SEVERITY_INFO, adapters[adapterIndex].VendorName);
	log.Write(LOG_SEVERITY_INFO, adapters[adapterIndex].ModelName);

	constexpr int32 bufferCount = 2;
	ISwapchain* swapchain = nullptr;
	IDevice* device = nullptr;
	ICommandQueue* queue = nullptr;

	DeviceInfo dInfo = {};
	dInfo.Name = "Main Device";
	dInfo.pAdapter = &(adapters[adapterIndex]);

	SwapchainInfo scInfo = {};
	scInfo.pWindow = &window;
	scInfo.Buffer.Count = bufferCount;
	scInfo.Buffer.Format = FORMAT_B8G8R8A8_UNORM;
	scInfo.Buffer.Width = window.GetWidth();
	scInfo.Buffer.Height = window.GetHeight();

	if (factory->GetGraphicsApi() != GRAPHICS_API_VULKAN)
	{
		factory->CreateDevice(&device, dInfo);

		CommandQueueInfo qInfo = {};
		qInfo.Name = "Main Queue";
		device->CreateCommandQueue(&queue, qInfo);
		scInfo.pCommandQueue = queue;

		factory->CreateSwapchain(&swapchain, scInfo);
	}
	else
	{
		factory->CreateDeviceAndSwapchain(&device, dInfo, &swapchain, scInfo);
	}


	RenderTargetViewInfo rtvInfo = {};
	rtvInfo.Format = scInfo.Buffer.Format;
	
	IRenderTargetView* rtvs[bufferCount];
	for (int32 i = 0; i < bufferCount; i++)
	{
		rtvInfo.pResource = swapchain->GetBuffer(i);
		device->CreateRenderTargetView(&(rtvs[i]), rtvInfo);
	}


	IShaderCompiler* compiler = nullptr;
	factory->CreateShaderCompiler(&compiler);

	ShaderCompileInfo sInfo = {};
	sInfo.EntryPoint = "main";
	sInfo.SrcLang = SHADER_SOURCE_LANG_HLSL;
	sInfo.Type = SHADER_TYPE_VERTEX;
	ShaderByteCode vsCode = compiler->CompileFromFile("vs.hlsl", "Shaders/", sInfo);

	sInfo.Type = SHADER_TYPE_PIXEL;
	ShaderByteCode psCode = compiler->CompileFromFile("ps.hlsl", "Shaders/", sInfo);

	IShader* vs = nullptr;
	device->CreateShader(&vs, vsCode);

	IShader* ps = nullptr;
	device->CreateShader(&ps, psCode);

	IRootSignature* rootSignature = nullptr;
	RootSignatureInfo rootInfo = {};
	rootInfo.Name = "RootSignature";
	rootInfo.ParameterCount = 0;
	rootInfo.pParameters = nullptr;
	rootInfo.RootSignatureVisibility =
		ROOT_SIGNATURE_VISIBILITY_INPUT_LAYOUT |
		ROOT_SIGNATURE_VISIBILITY_VERTEX_SHADER |
		ROOT_SIGNATURE_VISIBILITY_PIXEL_SHADER;

	device->CreateRootSignature(&rootSignature, rootInfo);

	IPipelineState* pipelineState = nullptr;
	PipelineStateInfo pipelineInfo = {};
	pipelineInfo.Name = "PipelineState";
	pipelineInfo.Type = PIPELINE_TYPE_GRAPHICS;
	pipelineInfo.pRootSignature = rootSignature;
	pipelineInfo.GraphicsPipeline.pVertexShader = vs;
	pipelineInfo.GraphicsPipeline.pPixelShader = ps;

	InputElementInfo elementinfo = { "POSITION", 0, FORMAT_R32G32B32_FLOAT, ELEMENT_STEP_TYPE_VERTEX, 0, 0, 0 };
	pipelineInfo.GraphicsPipeline.InputLayout.ElementCount = 1;
	pipelineInfo.GraphicsPipeline.InputLayout.pElements = &elementinfo;

	device->CreatePipelineState(&pipelineState, pipelineInfo);


	TextureInfo dsInfo = {};
	dsInfo.Name = "DepthStencil-Texture";
	dsInfo.Flags = TEXTURE_FLAGS_DEPTHBUFFER;
	dsInfo.CpuAccess = CPU_ACCESS_FLAG_NONE;
	dsInfo.Usage = RESOURCE_USAGE_DEFAULT;
	dsInfo.Format = FORMAT_D16_UNORM;
	dsInfo.Type = TEXTURE_TYPE_2D;
	dsInfo.Width = window.GetWidth();
	dsInfo.Height = window.GetHeight();
	dsInfo.DepthOrArraySize = 1;
	dsInfo.MipLevels = 1;
	dsInfo.SampleCount = 1;

	ITexture* depthStencil = nullptr;
	device->CreateTexture(&depthStencil, nullptr, dsInfo);


	TextureInfo textureInfo = {};
	textureInfo.Name = "Texture";
	textureInfo.Flags = TEXTURE_FLAGS_TEXTURE;
	textureInfo.CpuAccess = CPU_ACCESS_FLAG_NONE;
	textureInfo.Usage = RESOURCE_USAGE_DEFAULT;
	textureInfo.Format = FORMAT_R8G8B8A8_UNORM;
	textureInfo.Type = TEXTURE_TYPE_2D;
	textureInfo.Width = 512;
	textureInfo.Height = 512;
	textureInfo.DepthOrArraySize = 1;
	textureInfo.MipLevels = 1;
	textureInfo.SampleCount = 1;

	ResourceData textureData = {};
	textureData.ByteStride = sizeof(uint8) * 4;
	textureData.WidthOrCount = 512;
	textureData.Height = 512;


	TextureLoader::LoadFromFile("chess.jpg", "Textures/", &textureData.pData, 
		textureData.WidthOrCount, textureData.Height, FORMAT_R8G8B8A8_UNORM);
	
	ITexture* texture = nullptr;
	device->CreateTexture(&texture, &textureData, textureInfo);


	DepthStencilViewInfo dsvInfo = {};
	dsvInfo.pResource = depthStencil;

	IDepthStencilView* dsv = nullptr;
	device->CreateDepthStencilView(&dsv, dsvInfo);

	queue->Reset();
	
	queue->TransitionResource(depthStencil, RESOURCE_STATE_DEPTH_WRITE, 0);

	queue->Close();
	queue->Execute();
	queue->Flush();


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

	BufferInfo vbInfo = {};
	vbInfo.Name = "VertexBuffer";
	vbInfo.Count = 3;
	vbInfo.ByteStride = sizeof(Vector3);
	vbInfo.Type = BUFFER_USAGE_VERTEX;
	vbInfo.Usage = RESOURCE_USAGE_DEFAULT;

	IBuffer* vertexBuffer = nullptr;
	device->CreateBuffer(&vertexBuffer, &vbData, vbInfo);

	window.Show();


	Clock clock;
	RandomGenerator ran;
	

	Math::Vector3 lastAccelerometer;


	float strength = 1.0f;
	ColorF bgColor = ColorF::CORNFLOWERBLUE;

	if (Sensors::SensorSupported(SENSOR_TYPE_ACCELEROMETER))
	{
		log.Write(LOG_SEVERITY_INFO, "Accelerometer supported");

		Sensors::EnableSensor(SENSOR_TYPE_ACCELEROMETER);
		
		if (!Sensors::SetRefreshRate(SENSOR_TYPE_ACCELEROMETER, TimeStamp::Seconds(0.5)))
			log.Write(LOG_SEVERITY_WARNING, "Could not set Sensor refreshrate");
	}


	while (event.Type != EVENT_TYPE_QUIT)
	{
		clock.Tick();

		while (window.PeekEvent(event))
		{
			if (event.Type == EVENT_TYPE_CLOSE)
				window.SendQuitEvent(0);

			if (event.Type == EVENT_TYPE_TOUCHPRESSED)
			{
				if (event.Touch.Position.x < (info.ScreenWidth / 2))
					Keyboard::HideVirtualKeyboardVisible();
				else
					Keyboard::ShowVirtualKeyboardVisible();
			}

			if (event.Type == EVENT_TYPE_FOCUSCHANGED)
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

			if (event.Type == EVENT_TYPE_KEYPRESSED)
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

			if (event.Type == EVENT_TYPE_SENSORCHANGED)
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
		}

		//if (clock.GetTotalTickTime().GetAsSeconds() > 0.01)
		//{
		//	ColorF c = bgColor * strength;
		//	window.SetBackground((Color)c);
		//	clock.Reset();
		//}

		queue->Reset();

		int32 buffIndex = swapchain->GetCurrentBuffer();
		ITexture* pBuff = swapchain->GetBuffer(buffIndex);
		queue->TransitionResource(pBuff, RESOURCE_STATE_RENDER_TARGET, 0);

		ColorF c = ColorF::DARKGREEN;
		queue->ClearRendertargetView(rtvs[swapchain->GetCurrentBuffer()], c);
		queue->ClearDepthStencilView(dsv, 1.0f, 0);

		Viewport vPort = {};
		vPort.Height = static_cast<float>(window.GetHeight());
		vPort.Width = static_cast<float>(window.GetWidth());
		vPort.TopLeftX = 0.0f;
		vPort.TopLeftY = 0.0f;
		vPort.MinDepth = 0.0f;
		vPort.MaxDepth = 1.0f;

		queue->SetViewports(vPort);

		Math::Rectangle rect = {};
		rect.BottomRight.x = static_cast<float>(window.GetWidth());
		rect.BottomRight.y = static_cast<float>(window.GetHeight());

		queue->SetScissorRects(rect);

		queue->SetRendertargets(rtvs[buffIndex], dsv);
		queue->SetPipelineState(pipelineState);
		queue->SetRootSignature(rootSignature);

		queue->SetPrimitiveTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		queue->SetVertexBuffers(vertexBuffer, 0);
		queue->Draw(0, 3);

		queue->TransitionResource(pBuff, RESOURCE_STATE_PRESENT, 0);

		queue->Close();
		queue->Execute();

		swapchain->Present();

		queue->Flush();
	}

	queue->Flush();

	ReRelease_S(texture);
	delete textureData.pData;

	ReRelease_S(vs);
	ReRelease_S(ps);
	ReRelease_S(vertexBuffer);
	ReRelease_S(dsv);
	ReRelease_S(rootSignature);
	ReRelease_S(pipelineState);
	ReRelease_S(compiler);
	ReRelease_S(depthStencil);

	for (int32 i = 0; i < bufferCount; i++)
	{
		ReRelease_S(rtvs[i]);
	}

	ReRelease_S(swapchain);
	ReRelease_S(queue);

	ReRelease_S(device);
	ReRelease_S(factory);

	return 0;
}