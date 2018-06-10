#include "..\..\Include\Android\AndroidWindowImpl.h"
#include "..\..\Include\System\Clock.h"

#if defined(RE_PLATFORM_ANDROID)

#include "..\..\Include\Android\Android.h"
#include <android\window.h>
#include <mutex>

namespace RayEngine
{
	namespace System
	{
		AndroidWindowImpl::AndroidWindowImpl()
			: m_Events(),
			m_Width(0),
			m_Height(0),
			m_Color(0),
			m_Flags(0),
			m_WindowID(CreateWindowID())
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
			m_Color = AndroidGetIntColor(desc.Color.r, desc.Color.g, desc.Color.b, 255);

			//Set window flags
			//if (desc.Style == WINDOWSTYLE_FULLSCREEN)
			//	m_Flags |= AWINDOW_FLAG_FULLSCREEN;

			return false;
		}

		void AndroidWindowImpl::Show() const
		{
			//Set the current window size so that when we get a new window we set it
			//in the callbackfunction and not in the message loop
			AndroidSetNativeWindowSize(m_Width, m_Height);

			//Set the current window color so that when we get a new window we set it
			//in the callbackfunction and not in the message loop
			AndroidSetNativeWindowColor(m_Color);

			//Set flags if there are any
			if (m_Flags != 0)
				AndroidSetNativeWindowFlags(m_Flags, 0);
		}

		void AndroidWindowImpl::PeekEvents()
		{
			ANDROID_EVENT osEvent = AndroidReciveEvent();
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
			AndroidSendEvent(ANDROID_EVENT_DESTROY);
		}

		void AndroidWindowImpl::SetCursor(const Cursor& cursor)
		{
			//Not relevant
		}

		void AndroidWindowImpl::SetIcon(const Icon& icon)
		{
			//Not relevant
		}

		void AndroidWindowImpl::SetBackground(uint8 r, uint8 g, uint8 b)
		{
			m_Color = AndroidGetIntColor(r, g, b, 255);
			AndroidSetNativeWindowColor(m_Color);
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



		/////////////////////////////////////////////////////////////



		//Global variable for keeping count of windows and its mutex
		int32 g_WindowCount = 0;
		std::mutex g_CountMutex;

		int32 AndroidWindowImpl::CreateWindowID()
		{
			//Lock g_WindowCount
			g_CountMutex.lock();

			//increase count
			g_WindowCount++;
			int32 res = g_WindowCount;

			//Unlock g_WindowCount
			g_CountMutex.unlock();

			return res;
		}



		/////////////////////////////////////////////////////////////
	}
}

#endif