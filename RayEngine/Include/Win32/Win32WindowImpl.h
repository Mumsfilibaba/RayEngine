#pragma once

#include "..\..\Include\Defines.h"
#if defined(RE_PLATFORM_WINDOWS)

#include "..\System\Window.h"
#include <queue>
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

#define WNDCLASS_NAME RE_T("Std_Window_Class")

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

			bool Create(const WindowDesc& desc) override final;

			void Show() const override final;

			void PeekEvents() override final;
			void PushEvent(const Event& pEvent) override final;
			bool PopEvent(Event& pEvent) override final;
			void GetEvent(Event& pEvent) override final;
			void SendQuitEvent(int32 exitCode) const override final;
			
			void SetCursor(const Cursor& cursor) override final;
			void SetIcon(const Icon& icon) override final;
			void SetBackground(uint8 r, uint8 g, uint8 b) override final;

			IWindowImpl* Copy() const override final;
			IWindowImpl* Move() override final;
			
			const Tchar* GetTitle() const override final;

			virtual int32 GetWidth() const override final;
			virtual int32 GetHeight() const override final;

			void GetDesc(WindowDesc& desc) const override final;

			HWND GetHWND() const;
			HINSTANCE GetHINSTANCE() const;

			LRESULT MsgCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		private:
			mutable Tchar m_Title[256];
			HWND m_Hwnd;
			HINSTANCE m_Hinstance;
			HBRUSH m_BgBrush;
			Cursor m_Cursor;
			std::queue<Event> m_Events;

		private:
			static LRESULT CALLBACK MainMsgCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		private:
			static int32 s_WindowCount;
		};
	}
}

#endif