#pragma once

#include "..\System\Window.h"

#if defined(RE_PLATFORM_ANDROID)

#include <queue>
#include <android/native_window.h>

namespace RayEngine
{
	namespace System
	{
		class AndroidWindowImpl : public IWindowImpl
		{
		public:
			AndroidWindowImpl(const AndroidWindowImpl& other) = delete;
			AndroidWindowImpl(AndroidWindowImpl&& other) = delete;
			AndroidWindowImpl& operator=(const AndroidWindowImpl& other) = delete;
			AndroidWindowImpl& operator=(AndroidWindowImpl&& other) = delete;

			AndroidWindowImpl();
			~AndroidWindowImpl();

			bool Create(const WindowDesc& desc) override final;

			void Show() const override final;

			void PeekEvents() override final;
			void PushEvent(const Event& pEvent) override final;
			bool PopEvent(Event& pEvent) override final;
			void GetEvent(Event& pEvent) override final;
			void SendQuitEvent(int32 exitCode) const override final;

			void SetCursor(const Cursor& cursor) override final;
			void SetIcon(const Icon& icon) override final;

			IWindowImpl* Copy() const override final;
			IWindowImpl* Move() override final;

			const Tchar* GetTitle() const override final;

			virtual int32 GetWidth() const override final;
			virtual int32 GetHeight() const override final;

			void GetDesc(WindowDesc& desc) const override final;

		private:
			std::queue<Event> m_Events;
			int32 m_Width;
			int32 m_Height;
			int32 m_Color;
			int32 m_Flags;
		};
	}
}

#endif