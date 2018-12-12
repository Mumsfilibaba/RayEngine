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
#include "RayEngine.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "System/IWindow.h"

#define WNDCLASS_NAME RE_T("Win32WindowImpl")

namespace RayEngine
{
	class Win32WindowImpl final : public IWindow
	{
		RE_IMPLEMENT_INTERFACE(Win32WindowImpl);

	public:
		Win32WindowImpl(const WindowDesc* pDesc);
		~Win32WindowImpl();

		inline HWND GetHWND() const
		{
			return m_Hwnd;
		}
			
		inline HINSTANCE GetHINSTANCE() const
		{
			return m_Hinstance;
		}

		void Show() const override final;

		void Close() const override final;

		bool PeekEvent(Event* pEvent) const override final;

		void GetEvent(Event* pEvent) const override final;

		void SetIcon(const Image* pIcon) override final;

		void SetCursor(const Image* pCursor, const Math::Point& hotspot) override final;

		void SetTitle(const char* pTitle) override final;
			
		void SetBackground(uint8 r, uint8 g, uint8 b) override final;

		int32 GetWidth() const override final;
			
		int32 GetHeight() const override final;

		void GetDesc(WindowDesc* pDesc) const override final;

		void Destroy() override final;

		void GetNativeWindowHandle(NativeWindowHandle* pHandle) const override final;

		LRESULT MsgCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		void Create(const WindowDesc* pDesc);

		void ProcessMouseEvent(UINT msg, WPARAM wParam, LPARAM lParam);

		inline void PushEvent(const Event& event) const
		{
			m_Events.push(event);
		}

		void PopEvent(Event* pEvent) const 
		{
			if (!m_Events.empty())
			{
				*pEvent = m_Events.front();
				m_Events.pop();
			}
		}

	private:
		WindowDesc m_Desc;

		mutable std::queue<Event> m_Events;
			
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

#endif