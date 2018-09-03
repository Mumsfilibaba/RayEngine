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

#pragma once
#include "..\..\Include\Defines.h"
#if defined(RE_PLATFORM_WINDOWS)

#include "..\System\Window.h"
#include <queue>

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN 1
#endif

#include <Windows.h>

#define WNDCLASS_NAME RE_T("Win32WindowImpl")

namespace RayEngine
{
	namespace System
	{
		class RE_API Win32WindowImpl : public IWindowImpl
		{
		public:
			Win32WindowImpl(const Win32WindowImpl& other) = delete;
			Win32WindowImpl(Win32WindowImpl&& other) = delete;
			Win32WindowImpl& operator=(const Win32WindowImpl& other) = delete;
			Win32WindowImpl& operator=(Win32WindowImpl&& other) = delete;

			Win32WindowImpl();
			~Win32WindowImpl();

			bool Create(const WindowInfo& desc) override final;

			void Show() const override final;

			void PeekEvents() override final;
			void PushEvent(const Event& pEvent) override final;
			bool PopEvent(Event& pEvent) override final;
			void GetEvent(Event& pEvent) override final;
			void SendQuitEvent(int32 exitCode) const override final;
			
			void SetIcon(const Bitmap& icon) override final;
			void SetCursor(const Bitmap& cursor, int32 hotspotX, int32 hotspotY) override final;
			void SetBackground(uint8 r, uint8 g, uint8 b) override final;
			void SetBackground(const Math::Color& color) override final;

			IWindowImpl* Copy() const override final;
			
			const Tchar* GetTitle() const override final;

			virtual int32 GetWidth() const override final;
			virtual int32 GetHeight() const override final;

			void GetInfo(WindowInfo& desc) const override final;

			HWND GetHWND() const;
			HINSTANCE GetHINSTANCE() const;

			LRESULT MsgCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		private:
			void ProcessMouseEvent(UINT msg, WPARAM wParam, LPARAM lParam);

		private:
			mutable Tchar m_Title[256];
			std::queue<Event> m_Events;
			HWND m_Hwnd;
			HINSTANCE m_Hinstance;
			HBRUSH m_BgBrush;
			HICON m_Icon;
			HCURSOR m_Cursor;
			bool m_TrackingMouse;

		private:
			static LRESULT CALLBACK MainMsgCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		};
	}
}

#endif