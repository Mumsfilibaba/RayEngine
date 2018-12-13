/*////////////////////////////////////////////////////////////

Copyright 2018 Alexander Dahlin

Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in
compliance with the License. You may obtain a copy of
the License at

http ://www.apache.org/licenses/LICENSE-2.0

THIS SOFTWARE IS PROVIDED "AS IS". MEANING NO WARRANTY
OR SUPPORT IS PROVIDED OF ANY KIND.

In event of any damages, direct or indirect that can
be traced back to the use of this software, shall no
contributor be held liable. This includes computer
failure and or malfunction of any kind.

////////////////////////////////////////////////////////////*/

#include <RayEngine.h>

#if defined(RE_PLATFORM_WINDOWS)
#include <System/Mouse.h>
#include <Win32/Win32WindowImpl.h>

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Math::Point Mouse::GetPosition()
	{
		POINT mouse = {};
		GetCursorPos(&mouse);

		return Math::Point(mouse.x, mouse.y);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Math::Point Mouse::GetPosition(const IWindow* pWindow)
	{
		POINT mouse = {};
		GetCursorPos(&mouse);

		ScreenToClient(reinterpret_cast<const Win32WindowImpl*>(pWindow)->GetHWND(), &mouse);

		return Math::Point(mouse.x, mouse.y);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Mouse::IsButtonDown(MOUSEBUTTON button)
	{
		int32 vk = 0;
		switch (button)
		{
		case MOUSEBUTTON_LEFT:		vk = VK_LBUTTON; break;
		case MOUSEBUTTON_MIDDLE:	vk = VK_MBUTTON; break;
		case MOUSEBUTTON_RIGHT:		vk = VK_RBUTTON; break;
		case MOUSEBUTTON_BACK:		vk = VK_XBUTTON1; break;
		case MOUSEBUTTON_FORWARD:	vk = VK_XBUTTON2; break;
		default: vk = 0; break;
		}

		return GetAsyncKeyState(vk) & (1 << 16);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Mouse::SetPosition(const Math::Point& position)
	{
		SetCursorPos(position.x, position.y);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Mouse::SetPosition(const Math::Point& position, const IWindow* pWindow)
	{
		POINT mouse = {};
		mouse.x = position.x;
		mouse.y = position.y;

		ClientToScreen(reinterpret_cast<const Win32WindowImpl*>(pWindow)->GetHWND(), &mouse);

		SetCursorPos(mouse.x, mouse.y);
	}
}

#endif