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

#include "../../Include/Win32/Win32WindowImpl.h"

#if defined (RE_PLATFORM_WINDOWS)

#include "../../Include/Debug/Debug.h"
#include "../../Include/Utilities/EngineUtilities.h"
#include "../../Include/Utilities/TextureUtilities.h"
#include "Win32KeyCodes.h"
#include "WndclassCache.h"
#include <windowsx.h>

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Win32WindowImpl::Win32WindowImpl(const WindowDesc* pDesc)
		: IWindow(),
		m_Desc(),
		m_Hwnd(0),
		m_Hinstance(0),
		m_BgBrush(0),
		m_Cursor(0),
		m_Icon(0),
		m_Events(),
		m_TrackingMouse(false)
	{
		Create(pDesc);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Win32WindowImpl::~Win32WindowImpl()
	{
		if (IsWindow(m_Hwnd))
		{
			DestroyWindow(m_Hwnd);
			m_Hwnd = 0;
		}

		if (m_Cursor != 0)
		{
			DestroyCursor(m_Cursor);
			m_Cursor = 0;
		}

		if (m_Icon != 0)
		{
			DestroyIcon(m_Icon);
			m_Icon = 0;
		}

		if (m_BgBrush != 0)
		{
			DeleteObject(m_BgBrush);
			m_BgBrush = 0;
		}

		WndclassCache::Unregister(WNDCLASS_NAME, m_Hinstance);
		m_Hinstance = 0;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Win32WindowImpl::PeekEvent(Event* pEvent) const
	{
		MSG msg = {};
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				Event event;
				event.Type = EVENT_TYPE_QUIT;
				event.Quit.ExitCode = static_cast<int32>(msg.wParam);

				PushEvent(event);
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (m_Events.empty())
		{
			return false;
		}

		PopEvent(pEvent);
		return true;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Win32WindowImpl::GetEvent(Event* pEvent) const
	{
		MSG msg = {};
		if (GetMessage(&msg, 0, 0, 0) != 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			PopEvent(pEvent);
		}
		else
		{
			pEvent->Type = EVENT_TYPE_QUIT;
			pEvent->Quit.ExitCode = static_cast<int32>(msg.wParam);
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Win32WindowImpl::SetCursor(const Image* pCursor, const Math::Point& hotspot)
	{
		FORMAT format = pCursor->GetFormat();
		if (FormatStride(format) != 4 && FormatComponentCount(format) != 4)
		{
			LOG_ERROR("Format needs to be a 8-bit RGBA value");
			return;
		}

		if (m_Cursor != 0)
		{
			DestroyCursor(m_Cursor);
			m_Cursor = 0;
		}

		if (m_Desc.Cursor.pImage != nullptr)
		{
			delete m_Desc.Cursor.pImage;
			m_Desc.Cursor.pImage = nullptr;
		}

		int32 width = pCursor->GetWidth();
		int32 height = pCursor->GetHeight();

		void* pPixels = nullptr;
		pCursor->GetPixels(&pPixels);

		HBITMAP hMask = CreateBitmap(width, height, 1, 1, 0);
		HBITMAP hBitmap = CreateBitmap(width, height, 1, 32, reinterpret_cast<const void*>(pPixels));

		ICONINFO info = {};
		info.fIcon = FALSE;
		info.xHotspot = hotspot.x;
		info.yHotspot = hotspot.y;
		info.hbmColor = hBitmap;
		info.hbmMask = hMask;

		m_Cursor = (info.hbmColor == 0 || info.hbmMask == 0) ? 0 : reinterpret_cast<HCURSOR>(CreateIconIndirect(&info));

		DeleteObject(static_cast<HGDIOBJ>(hBitmap));
		DeleteObject(static_cast<HGDIOBJ>(hMask));

		m_Desc.Cursor.pImage = new Image(*pCursor);

		delete pPixels;
		pPixels = nullptr;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Win32WindowImpl::SetIcon(const Image* pIcon)
	{
		FORMAT format = pIcon->GetFormat();
		if (FormatStride(format) != 4 && FormatComponentCount(format) != 4)
		{
			LOG_ERROR("Format needs to be a 8-bit RGBA value");
			return;
		}


		if (m_Icon != 0)
		{
			DestroyIcon(m_Icon);
			m_Icon = 0;
		}

		if (m_Desc.pIcon != nullptr)
		{
			delete m_Desc.pIcon;
			m_Desc.pIcon = nullptr;
		}
			
		int32 width = pIcon->GetWidth();
		int32 height = pIcon->GetHeight();

		void* pPixels = nullptr;
		pIcon->GetPixels(&pPixels);

		if (format == FORMAT_R8G8B8A8_UNORM || format == FORMAT_R8G8B8A8_SINT ||
			format == FORMAT_R8G8B8A8_SNORM || format == FORMAT_R8G8B8A8_UINT ||
			format == FORMAT_R8G8B8A8_UNORM_SRGB)
		{
			ReverseImageRedBlue(pPixels, width, height, format);
		}

		HBITMAP hMask = CreateBitmap(width, height, 1, 1, 0);
		HBITMAP hBitmap = CreateBitmap(width, height, 1, 32, reinterpret_cast<const void*>(pPixels));

		ICONINFO info = {};
		info.fIcon = TRUE;
		info.hbmColor = hBitmap;
		info.hbmMask = hMask;

		m_Icon = (info.hbmColor == 0 || info.hbmMask == 0) ? 0 : CreateIconIndirect(&info);

		SendMessage(m_Hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(m_Icon));
		SendMessage(m_Hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(m_Icon));

		DeleteObject(static_cast<HGDIOBJ>(hBitmap));
		DeleteObject(static_cast<HGDIOBJ>(hMask));

		m_Desc.pIcon = new Image(*pIcon);

		delete pPixels;
		pPixels = nullptr;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Win32WindowImpl::SetTitle(const std::string& title)
	{
		SetWindowText(m_Hwnd, title.c_str());
		m_Desc.Title = title;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Win32WindowImpl::SetBackground(uint8 r, uint8 g, uint8 b)
	{
		if (m_BgBrush != 0)
		{
			DeleteObject(m_BgBrush);
			m_BgBrush = 0;
		}

		m_Desc.BackgroundColor.r = r;
		m_Desc.BackgroundColor.g = g;
		m_Desc.BackgroundColor.b = b;

		m_BgBrush = CreateSolidBrush(RGB(r, g, b));

		InvalidateRect(m_Hwnd, nullptr, TRUE);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int32 Win32WindowImpl::GetWidth() const
	{
		RECT rect = {};
		GetClientRect(m_Hwnd, &rect);

		return rect.right - rect.left;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int32 Win32WindowImpl::GetHeight() const
	{
		RECT rect = {};
		GetClientRect(m_Hwnd, &rect);

		return rect.bottom - rect.top;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Win32WindowImpl::GetNativeWindowHandle(NativeWindowHandle * pHandle) const
	{
		*pHandle = m_Hwnd;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Win32WindowImpl::GetDesc(WindowDesc* pDesc) const
	{
		*pDesc = m_Desc;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Win32WindowImpl::Destroy()
	{
		delete this;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Win32WindowImpl::Create(const WindowDesc* pDesc)
	{
		if (IsWindow(m_Hwnd))
		{
			DestroyWindow(m_Hwnd);
			m_Hwnd = 0;

			WndclassCache::Unregister(WNDCLASS_NAME, m_Hinstance);
		}

		//Set the desciption of window, set the pointer to nullptr, will be set later
		m_Desc = *pDesc;
		m_Desc.pIcon = nullptr;
		m_Desc.Cursor.pImage = nullptr;

		int32 error = 0;

		WNDCLASSEX wndClass = {};
		wndClass.hInstance = m_Hinstance = static_cast<HINSTANCE>(GetModuleHandle(nullptr));
		wndClass.lpfnWndProc = Win32WindowImpl::MainMsgCallback;
		wndClass.hbrBackground = 0;
		wndClass.hCursor = 0;
		wndClass.hIcon = 0;
		wndClass.hIconSm = 0;
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wndClass.lpszClassName = WNDCLASS_NAME;

		WndclassCache::Register(wndClass);

		int32 windowStyle = 0;
		if (pDesc->Style == WINDOWSTYLE_BORDERLESS)
		{
			windowStyle = WS_POPUP;
		}
		else
		{
			windowStyle = WS_OVERLAPPED;
			if (pDesc->Style & WINDOWSTYLE_TITLEBAR)
				windowStyle |= WS_CAPTION;
			if (pDesc->Style & WINDOWSTYLE_RESIZEABLE)
				windowStyle |= WS_THICKFRAME;
			if (pDesc->Style & WINDOWSTYLE_MINIMIZABLE)
				windowStyle |= WS_MINIMIZEBOX | WS_SYSMENU;
			if (pDesc->Style & WINDOWSTYLE_MAXIMIZABLE)
				windowStyle |= WS_MAXIMIZEBOX | WS_SYSMENU;
		}

		RECT client = { 0, 0, static_cast<LONG>(pDesc->Width), static_cast<LONG>(pDesc->Height) };
		AdjustWindowRect(&client, windowStyle, false);

		int32 posX = pDesc->x;
		int32 posY = pDesc->y;
		const char* pTitle = m_Desc.Title.c_str();

		SetLastError(0);
		m_Hwnd = CreateWindowEx(0, WNDCLASS_NAME, pTitle, windowStyle, posX, posY, client.right - client.left, client.bottom - client.top, 0, 0, m_Hinstance, 0);
		if (m_Hwnd == 0)
		{
			error = GetLastError();

			LOG_ERROR("Could not create window");
			return;
		}
		else
		{
			SetWindowLongPtr(m_Hwnd, GWLP_USERDATA, reinterpret_cast<uintptr_t>(this));
				
			SetIcon(pDesc->pIcon);
			SetCursor(pDesc->Cursor.pImage, pDesc->Cursor.Hotspot);
			SetBackground(pDesc->BackgroundColor.r, pDesc->BackgroundColor.g, pDesc->BackgroundColor.b);
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Win32WindowImpl::Show() const
	{
		ShowWindow(m_Hwnd, SW_NORMAL);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Win32WindowImpl::Close() const
	{
		ShowWindow(m_Hwnd, SW_MINIMIZE);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	LRESULT Win32WindowImpl::MsgCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Event event = {};
		memset(&event, 0, sizeof(Event));

		LRESULT ret = 0;

		switch (msg)
		{
		case WM_SETCURSOR:
			if (LOWORD(lParam) == HTCLIENT)
			{
				if (m_Cursor != 0)
				{
					::SetCursor(m_Cursor);
					return TRUE;
				}
			}

			return DefWindowProc(hWnd, msg, wParam, lParam);


		case WM_ERASEBKGND:
		{
			HDC dc = reinterpret_cast<HDC>(wParam);
			RECT rect = {};

			SetLastError(0);
			GetClientRect(hWnd, &rect);
			int32 error = GetLastError();
				
			SetLastError(0);
			FillRect(dc, &rect, m_BgBrush);
			error = GetLastError();

			return static_cast<LRESULT>(1);
		}

		case WM_MOUSEWHEEL:
			event.Type = EVENT_TYPE_MOUSESCROLL;
			event.MouseScroll.Delta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / static_cast<float>(WHEEL_DELTA);
			break;

		case WM_KEYDOWN:
			event.Type = EVENT_TYPE_KEYPRESSED;
			event.Key.KeyCode = Win32ToRe(static_cast<int32>(wParam));
			event.Key.RepeatCount = LOWORD(lParam);
			event.Key.Extended = lParam & (1 << 24);
			break;

		case WM_KEYUP:
			event.Type = EVENT_TYPE_KEYRELEASED;
			event.Key.KeyCode = Win32ToRe(static_cast<int32>(wParam));
			event.Key.RepeatCount = 1;
			event.Key.Extended = lParam & (1 << 24);
			break;

		case WM_UNICHAR:
			if (wParam == UNICODE_NOCHAR)
				return TRUE;
		case WM_CHAR:
			event.Type = EVENT_TYPE_KEYCHAR;
			event.KeyChar.UnicodeChar = static_cast<uint32>(wParam);
			break;

		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_XBUTTONDOWN:
			ProcessMouseEvent(msg, wParam, lParam);
			return static_cast<LRESULT>(0);

		case WM_SIZE:
			event.Type = EVENT_TYPE_RESIZE;
			event.Resize.Width = LOWORD(lParam);
			event.Resize.Height = HIWORD(lParam);
			if (wParam == SIZE_MAXIMIZED)
				event.Resize.Type = EVENT_RESIZE_MAXIMIZED;
			else if (wParam == SIZE_MINIMIZED)
				event.Resize.Type = EVENT_RESIZE_MINIMIZED;
			else
				event.Resize.Type = EVENT_RESIZE_NEW_SIZE;
			break;

		case WM_ACTIVATE:
			event.Type = EVENT_TYPE_FOCUSCHANGED;
			event.FocusChanged.HasFocus = !(LOWORD(wParam) == WA_INACTIVE);
			break;

		case WM_DESTROY:
			event.Type = EVENT_TYPE_CLOSE;
			break;


		default: 
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		PushEvent(event);
		return ret;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Win32WindowImpl::ProcessMouseEvent(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Event event;

		switch (msg)
		{
		case WM_MOUSELEAVE:
			if (GetCapture() == m_Hwnd)
				ReleaseCapture();
				
			m_TrackingMouse = false;
			return;

		case WM_MOUSEMOVE:
			event.Type = EVENT_TYPE_MOUSEMOVE;
			event.MouseMove.Position = Math::Point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;

		case WM_LBUTTONDOWN:
			SetCapture(m_Hwnd);
				
			event.Type = EVENT_TYPE_MOUSEPRESSED;
			event.MouseButton.Button = MOUSEBUTTON_LEFT;
			break;

		case WM_MBUTTONDOWN:
			SetCapture(m_Hwnd);
				
			event.Type = EVENT_TYPE_MOUSEPRESSED;
			event.MouseButton.Button = MOUSEBUTTON_MIDDLE;
			break;

		case WM_RBUTTONDOWN:
			SetCapture(m_Hwnd);
				
			event.Type = EVENT_TYPE_MOUSEPRESSED;
			event.MouseButton.Button = MOUSEBUTTON_RIGHT;
			break;

		case WM_XBUTTONDOWN:
			SetCapture(m_Hwnd);

			event.Type = EVENT_TYPE_MOUSEPRESSED;
			event.MouseButton.Button = (HIWORD(wParam) == XBUTTON1) ? MOUSEBUTTON_BACK : MOUSEBUTTON_FORWARD;
			break;

		case WM_LBUTTONUP:
			ReleaseCapture();

			event.Type = EVENT_TYPE_MOUSERELEASED;
			event.MouseButton.Button = MOUSEBUTTON_LEFT;
			break;

		case WM_MBUTTONUP:
			ReleaseCapture();

			event.Type = EVENT_TYPE_MOUSERELEASED;
			event.MouseButton.Button = MOUSEBUTTON_MIDDLE;
			break;

		case WM_RBUTTONUP:
			ReleaseCapture();

			event.Type = EVENT_TYPE_MOUSERELEASED;
			event.MouseButton.Button = MOUSEBUTTON_RIGHT;
			break;

		case WM_XBUTTONUP:
			ReleaseCapture();
				
			event.Type = EVENT_TYPE_MOUSERELEASED;
			event.MouseButton.Button = (HIWORD(wParam) == XBUTTON1) ? MOUSEBUTTON_BACK : MOUSEBUTTON_FORWARD;
			break;
		}

		if (!m_TrackingMouse)
		{
			TRACKMOUSEEVENT tm = {};
			tm.cbSize = sizeof(TRACKMOUSEEVENT);
			tm.dwFlags = TME_LEAVE;
			tm.hwndTrack = m_Hwnd;
			TrackMouseEvent(&tm);

			m_TrackingMouse = true;
		}

		PushEvent(event);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	LRESULT Win32WindowImpl::MainMsgCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Win32WindowImpl* window = reinterpret_cast<Win32WindowImpl*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		return window->MsgCallback(hWnd, msg, wParam, lParam);
	}
}

#endif