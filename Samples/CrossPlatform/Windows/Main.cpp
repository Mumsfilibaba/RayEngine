#include <System/Window.h>
#include <System/Clock.h>
#include <System/Log.h>

int main(int args, char* argsv[])
{
	using namespace RayEngine;
	using namespace RayEngine::System;

	WindowInfo windowInfo = {};
	windowInfo.Color.r = 0;
	windowInfo.Color.g = 0;
	windowInfo.Color.b = 0;
	windowInfo.Flags = WINDOW_FLAG_APP_FULLSCREEN | WINDOW_FLAG_APP_NO_SLEEP;
#if defined(RE_PLATFORM_ANDROID)
	windowInfo.Width = 1080;
	windowInfo.Height = 1920;
#else
	windowInfo.Width = 432;
	windowInfo.Height = 768;
#endif

	Window window(windowInfo);

	Event event;

	window.Show();

	Clock clock;

	Log log;
	
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
				if (event.TouchX < (1080 / 2))
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