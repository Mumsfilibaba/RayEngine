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
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		AndroidWindowImpl::AndroidWindowImpl()
			: m_Events(),
			m_Width(0),
			m_Height(0),
			m_Color(0),
			m_Flags(0)
		{
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		AndroidWindowImpl::~AndroidWindowImpl()
		{
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool AndroidWindowImpl::Create(const WindowDesc& info)
		{
			//Set height and width
			m_Width = info.Width;
			m_Height = info.Height;

			//Set color
			m_Color = GetAndroidColor(info.BackgroundColor.r, info.BackgroundColor.g, info.BackgroundColor.b, 255);

			//Set window flags
			if (info.Flags & WINDOW_FLAG_APP_FULLSCREEN)
				m_Flags |= AWINDOW_FLAG_FULLSCREEN;
			if (info.Flags & WINDOW_FLAG_APP_NO_SLEEP)
				m_Flags |= AWINDOW_FLAG_KEEP_SCREEN_ON;

			return true;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
				AndroidSetNativeWindowFlags(m_Flags);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void AndroidWindowImpl::PeekEvents()
		{
			Event event = AndroidReciveEvent();
			if (event.Type != EVENT_TYPE_UNKNOWN)
			{
				m_Events.push(event);
			}
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void AndroidWindowImpl::PushEvent(const Event& pEvent)
		{
			m_Events.push(pEvent);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool AndroidWindowImpl::PopEvent(Event& pEvent)
		{
			if (m_Events.empty())
			{
				return false;
			}

			pEvent = m_Events.front();
			m_Events.pop();

			return true;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void AndroidWindowImpl::GetEvent(Event& pEvent)
		{
			do
			{
				PeekEvents();
			} while (m_Events.empty());

			PopEvent(pEvent);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void AndroidWindowImpl::SendQuitEvent(int32 exitCode) const
		{
			Event event;
			event.Type = EVENT_TYPE_QUIT;
			event.Quit.ExitCode = exitCode;

			AndroidSendEvent(event);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void AndroidWindowImpl::SetCursor(const Bitmap& cursor, int32 hotspotX, int32 hotspotY)
		{
			//Not relevant
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void AndroidWindowImpl::SetIcon(const Bitmap& icon)
		{
			//Not relevant
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void AndroidWindowImpl::SetBackground(const Math::Color& color)
		{
			SetBackground(color.r, color.g, color.b);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void AndroidWindowImpl::SetBackground(uint8 r, uint8 g, uint8 b)
		{
			m_Color = GetAndroidColor(r, g, b, 255);
			AndroidSetNativeWindowColor(m_Color);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IWindowImpl* AndroidWindowImpl::Copy() const
		{
			return nullptr;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		const Tchar* AndroidWindowImpl::GetTitle() const
		{
			//Not relevant
			return nullptr;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int32 AndroidWindowImpl::GetWidth() const
		{
			return m_Width;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int32 AndroidWindowImpl::GetHeight() const
		{
			return m_Height;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void AndroidWindowImpl::GetInfo(WindowDesc& info) const
		{
			info.Width = m_Width;
			info.Height = m_Height;

			info.BackgroundColor.r = (m_Color << 24) >> 24;
			info.BackgroundColor.g = (m_Color << 16) >> 24;
			info.BackgroundColor.b = (m_Color << 8) >> 24;

			info.Title = nullptr;

			info.x = 0;
			info.y = 0;

			return;
		}
	}
}

#endif