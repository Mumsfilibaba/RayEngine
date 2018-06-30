#include "..\..\Include\System\Mouse.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\Win32\Win32WindowImpl.h"

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN 1
#endif

#include <Windows.h>

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	Math::Point Mouse::GetPosition()
	{
		POINT mouse = {};
		GetCursorPos(&mouse);

		return Math::Point(mouse.x, mouse.y);
	}



	/////////////////////////////////////////////////////////////
	Math::Point Mouse::GetPosition(const System::Window& window)
	{
		POINT mouse = {};
		GetCursorPos(&mouse);

		const System::Win32WindowImpl* win = reinterpret_cast<const System::Win32WindowImpl*>(window.GetImplementation());
		HWND hWnd = win->GetHWND();

		ScreenToClient(hWnd, &mouse);

		return Math::Point(mouse.x, mouse.y);
	}



	/////////////////////////////////////////////////////////////
	bool Mouse::IsButtonPressed(MOUSEBUTTON button)
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



	/////////////////////////////////////////////////////////////
	void Mouse::SetPosition(const Math::Point& position)
	{
		SetCursorPos(position.x, position.y);
	}



	/////////////////////////////////////////////////////////////
	void Mouse::SetPosition(const Math::Point& position, const System::Window& window)
	{
		const System::Win32WindowImpl* win = reinterpret_cast<const System::Win32WindowImpl*>(window.GetImplementation());
		HWND hWnd = win->GetHWND();

		POINT mouse = {};
		mouse.x = position.x;
		mouse.y = position.y;

		ClientToScreen(hWnd, &mouse);

		SetCursorPos(mouse.x, mouse.y);
	}
}

#endif