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

	Clipboard::SetString("I can now set the clipboard string hehe");
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


	if (Sensors::SensorSupported(SENSOR_TYPE_ACCELEROMETER))
	{
		log.Write(LOG_SEVERITY_INFO, "Accelerometer supported");
		//Sensors::EnableSensor(SENSOR_TYPE_ACCELEROMETER);
	}

	if (Sensors::SensorSupported(SENSOR_TYPE_GYROSCOPE))
		log.Write(LOG_SEVERITY_INFO, "Gyroscope supported");

	if (Sensors::SensorSupported(SENSOR_TYPE_MAGNETIC_FIELD))
		log.Write(LOG_SEVERITY_INFO, "Magnetic Filed Sensor supported");


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
				int i = 0;
			}
		}

		if (clock.GetTotalTickTime().GetAsSeconds() > 0.01)
		{
			window.SetBackground(colors[color]);
			clock.Reset();
		}
	}

	return 0;
}