#include <System/Window.h>
#include <System/Clock.h>
#include <System/Log.h>
#include <System/System.h>
#include <System/Bitmap.h>
#include <System/Mouse.h>
#include <System/Keyboard.h>
#include <System/Clipboard.h>
#include <Math/RandomGenerator.h>



int main(int args, char* argsv[])
{
	using namespace RayEngine;
	using namespace RayEngine::System;
	using namespace Math;

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
	window.Show();

	Log log;
	Clock clock;
	RandomGenerator ran;

	log.Write(LOG_SEVERITY_INFO, Clipboard::GetString().c_str());
	log.Write(LOG_SEVERITY_INFO, Clipboard::GetString().c_str());

	int32 color = 0;
	uint8 strength = 0;

	Color colors[10] = 
	{
		Color::CORNFLOWERBLUE,
		Color::SOFTRED,
		Color::SOFTGREEN,
		Color::SOFTBLUE,
		Color::SOFTYELLOW,
		Color::SOFTGRAY,
		Color::RED,
		Color::GREEN,
		Color::BLUE,
		Color::WARMWHITE,
	};
	
	while (event.Type != EVENT_TYPE_QUIT)
	{
		clock.Tick();

		while (window.PeekEvent(event))
		{
			if (event.Type == EVENT_TYPE_CLOSE)
				window.SendQuitEvent(0);

			//if (event.Type == EVENT_TYPE_TOUCHPRESSED)
			//	window.SetBackground(Color::RED);

			//if (event.Type == EVENT_TYPE_TOUCHRELEASED)
			//	window.SetBackground(Color::CORNFLOWERBLUE);

			if (event.Type == EVENT_TYPE_TOUCHPRESSED)
			{
				if (event.Touch.Position.x < (info.ScreenWidth / 2))
					color--;
				else
					color++;

				if (color < 0)
					color = 9;
				else if (color > 9)
					color = 0;
			}

			if (event.Type == EVENT_TYPE_FOCUSCHANGED)
			{
				if (event.FocusChanged.HasFocus)
					window.SetBackground(Color::CORNFLOWERBLUE);
				else
					window.SetBackground(Color::RED);
			}

			if (event.Type == EVENT_TYPE_KEYPRESSED)
				log.Write(LOG_SEVERITY_INFO, std::to_string(event.Key.KeyCode).c_str());
		}

		if (clock.GetTotalTickTime().GetAsSeconds() > 0.01)
		{
			window.SetBackground(colors[color]);
			clock.Reset();
		}
	}

	return 0;
}