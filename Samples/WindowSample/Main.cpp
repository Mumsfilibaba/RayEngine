#include <iostream>
#include <System\Window.h>
#include <Graphics\TextureLoader.h>

#pragma comment(lib, "RayEngineFramework.lib")

int main()
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

	using namespace RayEngine;
	using namespace System;

	Cursor cursor;
	cursor.LoadFromFile(RE_T("Cursor.png"), RE_T(""), 0, 0);

	Icon icon;
	icon.LoadFromFile(RE_T("Walter.png"), RE_T(""));

	WindowDesc desc;
	desc.title = RE_T("WindowSample");
	desc.width = 1280;
	desc.height = 720;
	desc.x = -1;
	desc.y = -1;
	desc.color.r = 200;
	desc.color.g = 200;
	desc.color.b = 200;
	desc.icon = std::move(icon);
	desc.cursor = std::move(cursor);

	Window window(desc);
	Event ev;

	window.Show();

	while (ev.type != EVENT_TYPE_QUIT)
	{
		while (window.PeekEvent(ev))
		{
			if (ev.type == EVENT_TYPE_DESTROYED)
				window.SendQuitEvent(0);
			else if (ev.type == EVENT_TYPE_RESIZE)
				std::cout << "Window Resized: (W: " << ev.width << ", H: " << ev.height << ", Type: " << ev.resizeType << ")\n";
			else if (ev.type == EVENT_TYPE_KEYPRESSED)
				std::cout << "Key Pressed: " << ev.keyCode << ", Repeat: " << ev.keyRepeatCount << std::boolalpha << ", Extended: " << ev.keyExtended << '\n';
			else if (ev.type == EVENT_TYPE_KEYRELEASED)
				std::cout << "Key Released: " << ev.keyCode << ", Repeat: " << ev.keyRepeatCount << std::boolalpha << ", Extended: " << ev.keyExtended << '\n';
			else if (ev.type != EVENT_TYPE_UNKNOWN)
				std::cout << "UNKOWN MESSAGE\n";
		}
	}

	//std::cin.ignore();
	return 0;
}