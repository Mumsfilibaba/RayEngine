#include "..\..\Include\Win32\Win32WindowImpl.h"
#include "..\..\Include\Win32\Win32IconImpl.h"
#include "..\..\Include\Win32\Win32CursorImpl.h"

#if defined (RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace System
	{
		/////////////////////////////////////////////////////////////
		int32 Win32WindowImpl::s_WindowCount = 0;



		/////////////////////////////////////////////////////////////
		Win32WindowImpl::Win32WindowImpl()
			: IWindowImpl(),
			m_Hwnd(0),
			m_Hinstance(0),
			m_BgBrush(0),
			m_Cursor(),
			m_Title(),
			m_Events()
		{
			memset(m_Title, 0, sizeof(m_Title));
		}



		/////////////////////////////////////////////////////////////
		Win32WindowImpl::~Win32WindowImpl()
		{
			if (IsWindow(m_Hwnd))
			{
				DestroyWindow(m_Hwnd);
				m_Hwnd = 0;
			}

			if (m_BgBrush != 0)
				DeleteObject(m_BgBrush);


			//TODO: Have a classmanager if we get more classes

			s_WindowCount--;
			if (s_WindowCount < 1)
				UnregisterClass(WNDCLASS_NAME, m_Hinstance);

			m_Hinstance = 0;
		}



		/////////////////////////////////////////////////////////////
		void Win32WindowImpl::PeekEvents()
		{
			MSG msg = {};
			while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					Event ev;
					ev.Type = EVENT_TYPE_QUIT;
					ev.QuitCode = static_cast<int32>(msg.wParam);

					PushEvent(ev);
				}

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}



		/////////////////////////////////////////////////////////////
		void Win32WindowImpl::PushEvent(const Event& pEvent)
		{
			m_Events.push(pEvent);
		}



		/////////////////////////////////////////////////////////////
		bool Win32WindowImpl::PopEvent(Event& pEvent)
		{
			if (m_Events.empty())
				return false;

			pEvent = m_Events.front();
			m_Events.pop();

			return true;
		}



		/////////////////////////////////////////////////////////////
		void Win32WindowImpl::GetEvent(Event& pEvent)
		{
			MSG msg = {};
			if (GetMessage(&msg, 0, 0, 0) != 0)
			{
				//All other messages
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				PopEvent(pEvent);
			}
			else
			{
				//If the message is a quit message
				pEvent.Type = EVENT_TYPE_QUIT;
				pEvent.QuitCode = static_cast<int32>(msg.wParam);
			}
		}



		/////////////////////////////////////////////////////////////
		void Win32WindowImpl::SendQuitEvent(int32 exitCode) const
		{
			PostQuitMessage(exitCode);
		}



		/////////////////////////////////////////////////////////////
		void Win32WindowImpl::SetCursor(const Cursor& cursor)
		{
			m_Cursor = cursor;
		}



		/////////////////////////////////////////////////////////////
		void Win32WindowImpl::SetIcon(const Icon& icon)
		{
			const Win32IconImpl* impl = static_cast<const Win32IconImpl*>(icon.GetImplementation());
			if (impl != nullptr)
			{
				SendMessage(m_Hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(impl->GetHICON()));
				SendMessage(m_Hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(impl->GetHICON()));
			}
		}



		/////////////////////////////////////////////////////////////
		void Win32WindowImpl::SetBackground(uint8 r, uint8 g, uint8 b)
		{
			//Delete old brush
			if (m_BgBrush != 0)
				DeleteObject(m_BgBrush);

			//Make window redraw
			InvalidateRect(m_Hwnd, nullptr, TRUE);

			//Create new background brush
			m_BgBrush = CreateSolidBrush(RGB(r, g, b));
		}



		/////////////////////////////////////////////////////////////
		IWindowImpl* Win32WindowImpl::Copy() const
		{
			//Get desc
			WindowInfo info;
			GetDesc(info);

			//Create new instance and create a copy of window
			Win32WindowImpl* instance = new Win32WindowImpl();
			instance->Create(info);
			return instance;
		}



		/////////////////////////////////////////////////////////////
		IWindowImpl* Win32WindowImpl::Move()
		{
			Win32WindowImpl* instance = new Win32WindowImpl();
			//Set handles
			instance->m_Hwnd = m_Hwnd;
			instance->m_Hinstance = m_Hinstance;
			//Swap eventqueue
			instance->m_Events.swap(m_Events);

			//Invalidate handles
			m_Hwnd = 0;
			m_Hinstance = 0;
			return nullptr;
		}



		/////////////////////////////////////////////////////////////
		const Tchar* Win32WindowImpl::GetTitle() const
		{
			GetWindowText(m_Hwnd, m_Title, 256);
			return m_Title;
		}



		/////////////////////////////////////////////////////////////
		int32 Win32WindowImpl::GetWidth() const
		{
			RECT rect = {};
			GetClientRect(m_Hwnd, &rect);

			return rect.right - rect.left;
		}



		/////////////////////////////////////////////////////////////
		int32 Win32WindowImpl::GetHeight() const
		{
			RECT rect = {};
			GetClientRect(m_Hwnd, &rect);

			return rect.bottom - rect.top;
		}



		/////////////////////////////////////////////////////////////
		void Win32WindowImpl::GetDesc(WindowInfo& info) const
		{
			GetWindowText(m_Hwnd, m_Title, 256);
		}



		/////////////////////////////////////////////////////////////
		HWND Win32WindowImpl::GetHWND() const
		{
			return m_Hwnd;
		}



		/////////////////////////////////////////////////////////////
		HINSTANCE Win32WindowImpl::GetHINSTANCE() const
		{
			return m_Hinstance;
		}



		/////////////////////////////////////////////////////////////
		bool Win32WindowImpl::Create(const WindowInfo& info)
		{
			static bool registerWindow = true;

			int32 err = 0;

			m_BgBrush = CreateSolidBrush(RGB(info.Color.r, info.Color.g, info.Color.b));
			



			if (registerWindow)
			{
				WNDCLASSEX wndClass = {};
				wndClass.hInstance = m_Hinstance = static_cast<HINSTANCE>(GetModuleHandle(nullptr));
				wndClass.lpfnWndProc = Win32WindowImpl::MainMsgCallback;
				wndClass.hbrBackground = 0;
				wndClass.hCursor = 0;
				wndClass.hIcon = 0;
				wndClass.hIconSm = wndClass.hIcon;
				wndClass.cbSize = sizeof(WNDCLASSEX);
				wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
				wndClass.lpszClassName = WNDCLASS_NAME;

				SetLastError(0);
				if (RegisterClassEx(&wndClass) == 0)
				{
					err = GetLastError();
					return false;
				}

				registerWindow = false;
			}




			int32 windowStyle = 0;
			if (info.Style == WINDOWSTYLE_BORDERLESS)
			{
				windowStyle = WS_POPUP;
			}
			else
			{
				windowStyle = WS_OVERLAPPED;
				if (info.Style & WINDOWSTYLE_TITLEBAR)
					windowStyle |= WS_CAPTION;
				if (info.Style & WINDOWSTYLE_SYSTEMMENU)
					windowStyle |= WS_SYSMENU;
				if (info.Style & WINDOWSTYLE_RESIZEABLE)
					windowStyle |= WS_THICKFRAME;
				if (info.Style & WINDOWSTYLE_MINIMIZABLE)
					windowStyle |= WS_MINIMIZEBOX;
				if (info.Style & WINDOWSTYLE_MAXIMIZABLE)
					windowStyle |= WS_MAXIMIZEBOX;
			}

			

			RECT client = { 0, 0, static_cast<LONG>(info.Width), static_cast<LONG>(info.Height) };
			AdjustWindowRect(&client, windowStyle, false);

			int32 posX = (info.x >= 0) ? info.x : (GetSystemMetrics(SM_CXSCREEN) - (client.right - client.left)) / 2;
			int32 posY = (info.y >= 0) ? info.y : (GetSystemMetrics(SM_CYSCREEN) - (client.bottom - client.top)) / 2;

			


			SetLastError(0);
			m_Hwnd = CreateWindowEx(0, WNDCLASS_NAME, info.Title, windowStyle, posX, posY, client.right - client.left, client.bottom - client.top, 0, 0, m_Hinstance, 0);
			if (m_Hwnd == 0)
			{
				err = GetLastError();
				return false;
			}

			SetWindowLongPtr(m_Hwnd, GWLP_USERDATA, reinterpret_cast<uintptr_t>(this));
			
			

			const Win32IconImpl* icon = static_cast<const Win32IconImpl*>(info.icon.GetImplementation());
			SendMessage(m_Hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon->GetHICON()));
			SendMessage(m_Hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(icon->GetHICON()));

			

			m_Cursor = info.cursor;

			s_WindowCount++;
			return true;
		}



		/////////////////////////////////////////////////////////////
		void Win32WindowImpl::Show() const
		{
			if (IsWindow(m_Hwnd))
				ShowWindow(m_Hwnd, SW_NORMAL);
		}



		/////////////////////////////////////////////////////////////
		LRESULT Win32WindowImpl::MsgCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			Event ev;
			LRESULT ret = 0;

			switch (msg)
			{
			case WM_SETCURSOR:
				if (reinterpret_cast<HWND>(wParam) == m_Hwnd)
				{
					if (LOWORD(lParam) == HTCLIENT)
					{
						::SetCursor((static_cast<const Win32CursorImpl*>(m_Cursor.GetImplementation())->GetHCURSOR()));
						return TRUE;
					}
				}

				return DefWindowProc(hWnd, msg, wParam, lParam);
			case WM_ERASEBKGND:
			{
				//Paint the background with a custom color
				HDC dc = reinterpret_cast<HDC>(wParam);
				RECT rect = {};
				GetClientRect(hWnd, &rect);
				FillRect(dc, &rect, m_BgBrush);

				return static_cast<LRESULT>(1);
			}
			case WM_KEYDOWN:
				ev.Type = EVENT_TYPE_KEYPRESSED;
				ev.KeyCode = static_cast<KEY>(wParam);
				ev.KeyRepeatCount = LOWORD(lParam);
				ev.KeyExtended = lParam & (1 << 24);
				break;
			case WM_KEYUP:
				ev.Type = EVENT_TYPE_KEYRELEASED;
				ev.KeyCode = static_cast<KEY>(wParam);
				ev.KeyRepeatCount = 1;
				ev.KeyExtended = lParam & (1 << 24);
				break;
			case WM_SIZE:
				ev.Type = EVENT_TYPE_RESIZE;
				ev.Width = LOWORD(lParam);
				ev.Height = HIWORD(lParam);
				if (wParam == SIZE_MAXIMIZED)
					ev.ResizeType = EVENT_RESIZE_MAXIMIZED;
				else if (wParam == SIZE_MINIMIZED)
					ev.ResizeType = EVENT_RESIZE_MINIMIZED;
				else
					ev.ResizeType = EVENT_RESIZE_NEW_SIZE;
				break;
			case WM_DESTROY:
				ev.Type = EVENT_TYPE_DESTROYED;
				break;
			default: 
				return DefWindowProc(hWnd, msg, wParam, lParam);
			}

			PushEvent(ev);
			return ret;
		}



		/////////////////////////////////////////////////////////////
		LRESULT Win32WindowImpl::MainMsgCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			Win32WindowImpl* window = reinterpret_cast<Win32WindowImpl*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			if (window == nullptr)
				return DefWindowProc(hWnd, msg, wParam, lParam);;

			return window->MsgCallback(hWnd, msg, wParam, lParam);
		}
	}
}

#endif