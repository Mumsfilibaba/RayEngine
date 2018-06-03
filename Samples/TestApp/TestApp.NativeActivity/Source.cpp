#include <System/Window.h>

int main(int args, char* argsv[])
{
	using namespace RayEngine::System;
	
	//Describe window
	WindowDesc desc = {};
	desc.Color.r = 100;
	desc.Color.g = 149;
	desc.Color.b = 237;

#if defined(RE_PLATFORM_ANDROID)
	desc.Width = 720;
	desc.Height = 1280;
#else
	desc.Width = 1280;
	desc.Height = 720;
#endif

	//TODO: Window flags
	//TODO: Handle orientation changes android - Send correct messages
	desc.Style = WINDOWSTYLE_FULLSCREEN;

	//Create window
	Window window(desc);

	//Event
	Event event;

	//Show window
	window.Show();

	//Event loop
	while (event.Type != EVENT_TYPE_QUIT)
	{
		while (window.PeekEvent(event))
		{
			if (event.Type == EVENT_TYPE_DESTROYED)
				window.SendQuitEvent(0);
			else if (event.Type == EVENT_TYPE_APP_PAUSED)
			{
				while (event.Type != EVENT_TYPE_APP_RESUMED)
					window.GetEvent(event);
			}
		}
	}

	return 0;
}