#include "..\..\Include\Android\AndroidWindowImpl.h"

#if defined(RE_PLATFORM_ANDROID)

#include "..\..\Include\Android\Android.h"
#include <android\window.h>

namespace RayEngine
{
	namespace System
	{
		AndroidWindowImpl::AndroidWindowImpl()
			: m_Events(),
			m_Width(0),
			m_Height(0),
			m_Color(0),
			m_Flags(0)
		{
		}

		AndroidWindowImpl::~AndroidWindowImpl()
		{
		}

		bool AndroidWindowImpl::Create(const WindowDesc& desc)
		{
			//Set height and width
			m_Width = desc.Width;
			m_Height = desc.Height;

			//Set color
			m_Color = get_color(desc.Color.r, desc.Color.g, desc.Color.b, 255);

			//Set window flags
			if (desc.Style == WINDOWSTYLE_FULLSCREEN)
				m_Flags |= AWINDOW_FLAG_FULLSCREEN;

			return false;
		}

		void AndroidWindowImpl::Show() const
		{
			//Set the current window size so that when we get a new window we set it
			//in the callbackfunction and not in the message loop
			set_native_window_size(m_Width, m_Height);

			//Set the current window color so that when we get a new window we set it
			//in the callbackfunction and not in the message loop
			set_native_window_color(m_Color);

			//Set flags if there are any
			if (m_Flags != 0)
				set_native_window_flags(m_Flags, 0);
		}

		void AndroidWindowImpl::PeekEvents()
		{
			ANDROID_EVENT osEvent = recive_event();
			if (osEvent == ANDROID_EVENT_DESTROY)
				PushEvent(Event(EVENT_TYPE_QUIT, 0));
			else if (osEvent == ANDROID_EVENT_APP_PAUSE)
				PushEvent(Event(EVENT_TYPE_APP_PAUSED, 0));
			else if (osEvent == ANDROID_EVENT_APP_RESUME)
				PushEvent(Event(EVENT_TYPE_APP_RESUMED, 0));

			//TODO: Send events for on pause etc.

			//TODO: Get Android events
		}
		
		void AndroidWindowImpl::PushEvent(const Event& pEvent)
		{
			m_Events.push(pEvent);
		}
		
		bool AndroidWindowImpl::PopEvent(Event& pEvent)
		{
			if (m_Events.empty())
				return false;

			pEvent = m_Events.front();
			m_Events.pop();

			return true;
		}
		
		void AndroidWindowImpl::GetEvent(Event& pEvent)
		{
			do
			{
				PeekEvents();
			} while (m_Events.empty());

			PopEvent(pEvent);
		}
		
		void AndroidWindowImpl::SendQuitEvent(int32 exitCode) const
		{
		}

		void AndroidWindowImpl::SetCursor(const Cursor& cursor)
		{
			//Not relevant
		}

		void AndroidWindowImpl::SetIcon(const Icon& icon)
		{
			//Not relevant
		}
		
		IWindowImpl* AndroidWindowImpl::Copy() const
		{
			return nullptr;
		}

		IWindowImpl* AndroidWindowImpl::Move()
		{
			return nullptr;
		}
		
		const Tchar* AndroidWindowImpl::GetTitle() const
		{
			//Not relevant
			return nullptr;
		}

		int32 AndroidWindowImpl::GetWidth() const
		{
			return m_Width;
		}

		int32 AndroidWindowImpl::GetHeight() const
		{
			return m_Height;
		}

		void AndroidWindowImpl::GetDesc(WindowDesc& desc) const
		{
			desc.Width = m_Width;
			desc.Height = m_Height;

			desc.Color.r = (m_Color << 24) >> 24;
			desc.Color.g = (m_Color << 16) >> 24;
			desc.Color.b = (m_Color << 8) >> 24;

			desc.Title = nullptr;

			desc.x = 0;
			desc.y = 0;

			return;
		}
	}
}

#endif