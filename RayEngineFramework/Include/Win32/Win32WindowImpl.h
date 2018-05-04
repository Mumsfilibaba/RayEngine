#pragma once

#include "..\System\Window.h"
#include <queue>
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

namespace RayEngine
{
	namespace System
	{
		class Win32WindowImpl : public IWindowImpl
		{
		public:
			Win32WindowImpl(const Win32WindowImpl& other) = delete;
			Win32WindowImpl(Win32WindowImpl&& other) = delete;
			Win32WindowImpl& operator=(const Win32WindowImpl& other) = delete;
			Win32WindowImpl& operator=(Win32WindowImpl&& other) = delete;

			Win32WindowImpl();
			~Win32WindowImpl();

			HWND GetHWND() const;
			HINSTANCE GetHINSTANCE() const;

		private:
			HWND m_Hwnd;
			HINSTANCE m_Hinstance;
			std::queue<Event> m_Events;
		};
	}
}