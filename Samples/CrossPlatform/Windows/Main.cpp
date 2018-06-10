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
	windowInfo.Flags = WINDOW_FLAG_APP_FULLSCREEN;
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
	uint8 red = 0;

	Log log;

	while (event.Type != EVENT_TYPE_QUIT)
	{
		clock.Tick();

		//Change color
		if (clock.GetTotalTime().GetAsSeconds() > 0.01)
		{
			red++;
			if (red >= 255)
				red = 0;

			window.SetBackground(red, 0, 0);
			clock.Reset();
		}

		//Check events
		while (window.PeekEvent(event))
		{
			if (event.Type == EVENT_TYPE_DESTROYED)
				window.SendQuitEvent(0);
		}
	}

	return 0;
}