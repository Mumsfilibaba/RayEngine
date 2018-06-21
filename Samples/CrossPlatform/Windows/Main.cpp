#include <System/Window.h>
#include <System/Clock.h>
#include <System/Log.h>
#include <System/System.h>
#include <Math/RandomGenerator.h>
#include <Math/Matrix4.h>

#include <chrono>

int main(int args, char* argsv[])
{
	using namespace RayEngine;
	using namespace RayEngine::System;
	using namespace Math;

	SystemInfo info;
	QuerySystemInfo(info);

	WindowInfo windowInfo = {};
	windowInfo.Color.r = 0;
	windowInfo.Color.g = 0;
	windowInfo.Color.b = 0;
	windowInfo.Flags = WINDOW_FLAG_APP_FULLSCREEN | WINDOW_FLAG_APP_NO_SLEEP;
	windowInfo.Style = WINDOWSTYLE_STANDARD_WINDOW;
#if defined(RE_PLATFORM_ANDROID)
	windowInfo.Width = 1080;
	windowInfo.Height = 1920;
#else
	windowInfo.Width = 1280;
	windowInfo.Height = 720;
#endif

	Window window(windowInfo);

	Event event;

	window.Show();

	Log log;
	Clock clock;

	Math::Matrix4 v1(5);
	Math::Matrix4 v2(2);
	Math::Matrix4 v3;

	constexpr int32 tests = 1000;
	double total = 0.0;
	for (int i = 0; i < tests; i++)
	{
		clock.Tick();
		
		for (int x = 0; x < 2000000; x++)
		{
			v3 = v2 * v1;
		}

		clock.Tick();

		double delta = clock.GetDeltaTime().GetAsMilliSeconds();
		total += delta;

		log.Write(LOG_SEVERITY_INFO, v2.ToString().c_str());
		log.Write(LOG_SEVERITY_INFO, v3.ToString().c_str());
		log.Write(LOG_SEVERITY_INFO, (const Tchar*)(std::to_string(delta) + "ms").c_str());
	}

	log.Write(LOG_SEVERITY_INFO, (const Tchar*)("Average: " + std::to_string(total / double(tests)) + "ms").c_str());

	RandomGenerator ran;
	
	int32 color = 0;
	uint8 strength = 0;

	struct
	{
		uint8 r;
		uint8 g;
		uint8 b;
	} 
	colors[10] = 
	{
		{ 255, 0, 0},
		{ 0, 255, 0 },
		{ 0, 0, 255 },
		{ 100, 149, 237 },
		{ 255, 64, 64 },
		{ 255, 255, 64 },
		{ 83, 83, 255 },
		{ 255, 241, 224 },
		{ 64, 255, 64 },
		{ 255, 192, 203 },
	};

	while (event.Type != EVENT_TYPE_QUIT)
	{
		clock.Tick();

		//Check events
		while (window.PeekEvent(event))
		{
			if (event.Type == EVENT_TYPE_DESTROYED)
				window.SendQuitEvent(0);

			if (event.Type == EVENT_TYPE_TOUCH)
			{
				if (event.TouchPosition.x < (1080 / 2))
					color--;
				else
					color++;

				if (color < 0)
					color = 9;
				else if (color > 9)
					color = 0;
			}
		}

		//Change color
		if (clock.GetTotalTime().GetAsSeconds() > 0.01)
		{
			window.SetBackground(colors[color].r, colors[color].g, colors[color].b);

			clock.Reset();
		}
	}

	return 0;
}