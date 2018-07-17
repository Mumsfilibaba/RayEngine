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

	IFactory* factory = IFactory::Create(GRAPHICS_API_D3D12, true);
	
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

	IDevice* device = nullptr;
	DeviceInfo dInfo = {};
	dInfo.Adapter = &(adapters[adapterIndex]);

	factory->CreateDevice(&device, dInfo);

	CommanQueueInfo qInfo = {};
	ICommandQueue* queue = nullptr;
	device->CreateCommandQueue(&queue, qInfo);
	
	IFence* fence = nullptr;
	device->CreateFence(&fence);

	ISwapchain* swapchain = nullptr;
	SwapchainInfo scInfo = {};
	scInfo.Window = &window;
	scInfo.commandQueue = queue;
	scInfo.Buffer.Count = 2;
	scInfo.Buffer.Format = FORMAT_R8G8B8A8_UINT;
	scInfo.Buffer.Width = window.GetWidth();
	scInfo.Buffer.Height = window.GetHeight();

	factory->CreateSwapchain(&swapchain, scInfo);

	IShaderCompiler* compiler = nullptr;
	factory->CreateShaderCompiler(&compiler);

	ShaderCompileInfo sInfo = {};
	sInfo.EntryPoint = "main";
	sInfo.SrcLang = SHADER_SOURCE_LANG_HLSL;
	sInfo.Type = SHADERTYPE_VERTEX;
	ShaderByteCode vsCode = compiler->CompileFromFile("vs.hlsl", "Shaders/", sInfo);

	IShader* vs = nullptr;
	device->CreateShader(&vs, vsCode);

	window.Show();


	Clock clock;
	RandomGenerator ran;
	
	int32 color = 0;
	float strength = 1.0;
	Math::Vector3 lastAccelerometer;

	ColorF colors[10] = 
	{
		ColorF::CORNFLOWERBLUE,
		ColorF::SOFTRED,
		ColorF::SOFTGREEN,
		ColorF::SOFTBLUE,
		ColorF::SOFTYELLOW,
		ColorF::SOFTGRAY,
		ColorF::RED,
		ColorF::GREEN,
		ColorF::BLUE,
		ColorF::WARMWHITE,
	};


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
				if (event.FocusChanged.HasFocus)
					window.SetBackground(Color::CORNFLOWERBLUE);
				else
					window.SetBackground(Color::RED);
			}

			if (event.Type == EVENT_TYPE_KEYPRESSED)
			{
				if (event.Key.KeyCode == KEY_0)
					color = 0;
				else if (event.Key.KeyCode == KEY_1)
					color = 1;
				else if (event.Key.KeyCode == KEY_2)
					color = 2;
				else if (event.Key.KeyCode == KEY_3)
					color = 3;
				else if (event.Key.KeyCode == KEY_4)
					color = 4;
				else if (event.Key.KeyCode == KEY_5)
					color = 5;
				else if (event.Key.KeyCode == KEY_6)
					color = 6;
				else if (event.Key.KeyCode == KEY_7)
					color = 7;
				else if (event.Key.KeyCode == KEY_8)
					color = 8;
				else if (event.Key.KeyCode == KEY_9)
					color = 9;
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

		if (clock.GetTotalTickTime().GetAsSeconds() > 0.01)
		{
			ColorF c = colors[color] * strength;
			window.SetBackground((Color)c);
			clock.Reset();
		}
	}


	delete fence;
	delete queue;
	delete device;
	delete factory;

	return 0;
}