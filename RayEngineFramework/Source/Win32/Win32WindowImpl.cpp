#include "..\..\Include\Win32\Win32WindowImpl.h"
#include "..\..\Include\Win32\Win32IconImpl.h"
#include "..\..\Include\Win32\Win32CursorImpl.h"

namespace RayEngine
{
	namespace System
	{
		int32 Win32WindowImpl::s_WindowCount = 0;

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

		Win32WindowImpl::~Win32WindowImpl()
		{
			//Destroy window handle
			if (IsWindow(m_Hwnd))
			{
				DestroyWindow(m_Hwnd);
				m_Hwnd = 0;
			}

			//Destroy the background
			if (m_BgBrush != 0)
				DeleteObject(m_BgBrush);

			//Decrease windowcount and unregisterclass when last window is done
			//TODO: Have a classmanager if we get more classes

			s_WindowCount--;
			if (s_WindowCount < 1)
				UnregisterClass(WNDCLASS_NAME, m_Hinstance);

			m_Hinstance = 0;
		}

		void Win32WindowImpl::PeekEvents()
		{
			MSG msg = {};
			while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					Event ev;
					ev.type = EVENT_TYPE_QUIT;
					ev.value = static_cast<int64>(msg.wParam);

					PushEvent(ev);
				}

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		void Win32WindowImpl::PushEvent(const Event& pEvent)
		{
			m_Events.push(pEvent);
		}

		bool Win32WindowImpl::PopEvent(Event& pEvent)
		{
			if (m_Events.empty())
				return false;

			pEvent = m_Events.front();
			m_Events.pop();

			return true;
		}

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
				pEvent.type = EVENT_TYPE_QUIT;
				pEvent.value = static_cast<int64>(msg.wParam);
			}
		}

		void Win32WindowImpl::SendQuitEvent(int32 exitCode) const
		{
			PostQuitMessage(exitCode);
		}

		void Win32WindowImpl::SetCursor(const Cursor& cursor)
		{
			m_Cursor = cursor;
		}

		void Win32WindowImpl::SetIcon(const Icon& icon)
		{
			const Win32IconImpl* impl = static_cast<const Win32IconImpl*>(icon.GetImplementation());
			if (impl != nullptr)
			{
				SendMessage(m_Hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(impl->GetHICON()));
				SendMessage(m_Hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(impl->GetHICON()));
			}
		}

		IWindowImpl* Win32WindowImpl::Copy() const
		{
			//Get desc
			WindowDesc desc;
			GetDesc(desc);

			//Create new instance and create a copy of window
			Win32WindowImpl* instance = new Win32WindowImpl();
			instance->Create(desc);
			return instance;
		}

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

		const Tchar* Win32WindowImpl::GetTitle() const
		{
			GetWindowText(m_Hwnd, m_Title, 256);
			return m_Title;
		}

		int32 Win32WindowImpl::GetWidth() const
		{
			RECT rect = {};
			GetClientRect(m_Hwnd, &rect);

			return rect.right - rect.left;
		}

		int32 Win32WindowImpl::GetHeight() const
		{
			RECT rect = {};
			GetClientRect(m_Hwnd, &rect);

			return rect.bottom - rect.top;
		}

		void Win32WindowImpl::GetDesc(WindowDesc& desc) const
		{
			GetWindowText(m_Hwnd, m_Title, 256);
		}

		HWND Win32WindowImpl::GetHWND() const
		{
			return m_Hwnd;
		}

		HINSTANCE Win32WindowImpl::GetHINSTANCE() const
		{
			return m_Hinstance;
		}

		bool Win32WindowImpl::Create(const WindowDesc& desc)
		{
			static bool registerWindow = true;

			//System error code
			int32 err = 0;

			//Create background brush
			m_BgBrush = CreateSolidBrush(RGB(desc.color.r, desc.color.g, desc.color.b));
			
			//Register the windowclass
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

			//TODO: Custom style
			int32 windowStyle = WS_OVERLAPPEDWINDOW;

			RECT client = { 0, 0, static_cast<LONG>(desc.width), static_cast<LONG>(desc.height) };
			AdjustWindowRect(&client, windowStyle, false);

			int32 posX = (desc.x >= 0) ? desc.x : (GetSystemMetrics(SM_CXSCREEN) - (client.right - client.left)) / 2;
			int32 posY = (desc.y >= 0) ? desc.y : (GetSystemMetrics(SM_CYSCREEN) - (client.bottom - client.top)) / 2;

			//TODO: Custom style
			SetLastError(0);
			m_Hwnd = CreateWindowEx(0, WNDCLASS_NAME, desc.title, windowStyle, posX, posY, client.right - client.left, client.bottom - client.top, 0, 0, m_Hinstance, 0);
			if (m_Hwnd == 0)
			{
				err = GetLastError();
				return false;
			}

			//Set userdata to be a pointer to the implementation
			SetWindowLongPtr(m_Hwnd, GWLP_USERDATA, reinterpret_cast<uintptr_t>(this));
			
			//Set the icon
			const Win32IconImpl* icon = static_cast<const Win32IconImpl*>(desc.icon.GetImplementation());
			SendMessage(m_Hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon->GetHICON()));
			SendMessage(m_Hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(icon->GetHICON()));

			//Copy cursor
			m_Cursor = desc.cursor;

			//Increase windowcount
			s_WindowCount++;
			return true;
		}

		void Win32WindowImpl::Show() const
		{
			if (IsWindow(m_Hwnd))
				ShowWindow(m_Hwnd, SW_NORMAL);
		}

		LRESULT Win32WindowImpl::MsgCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			//System event
			Event ev;
			LRESULT ret = 0;

			//Check messages
			switch (msg)
			{
			case WM_SETCURSOR:
				//Set cursor when is enters the clientarea
				if (reinterpret_cast<HWND>(wParam) == m_Hwnd)
				{
					if (LOWORD(lParam) == HTCLIENT)
					{
						//Not this->SetCursor();
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
				ev.type = EVENT_TYPE_KEYPRESSED;
				ev.keyCode = static_cast<KEY>(wParam);
				ev.keyRepeatCount = LOWORD(lParam);
				ev.keyExtended = lParam & (1 << 24);
				break;
			case WM_KEYUP:
				ev.type = EVENT_TYPE_KEYRELEASED;
				ev.keyCode = static_cast<KEY>(wParam);
				ev.keyRepeatCount = 1;
				ev.keyExtended = lParam & (1 << 24);
				break;
			case WM_SIZE:
				ev.type = EVENT_TYPE_RESIZE;
				//New size
				ev.width = LOWORD(lParam);
				ev.height = HIWORD(lParam);
				//If it was maximized, minimized or normal resize
				if (wParam == SIZE_MAXIMIZED)
					ev.resizeType = EVENT_RESIZE_MAXIMIZED;
				else if (wParam == SIZE_MINIMIZED)
					ev.resizeType = EVENT_RESIZE_MINIMIZED;
				else
					ev.resizeType = EVENT_RESIZE_NEW_SIZE;
				break;
			case WM_DESTROY:
				ev.type = EVENT_TYPE_DESTROYED;
				break;
			default: 
				return DefWindowProc(hWnd, msg, wParam, lParam);
			}

			//Push to eventqueue and return proper returnvalue
			PushEvent(ev);
			return ret;
		}

		//Static callback
		LRESULT Win32WindowImpl::MainMsgCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			Win32WindowImpl* window = reinterpret_cast<Win32WindowImpl*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			if (window == nullptr)
				return DefWindowProc(hWnd, msg, wParam, lParam);;

			return window->MsgCallback(hWnd, msg, wParam, lParam);
		}
	}
}
