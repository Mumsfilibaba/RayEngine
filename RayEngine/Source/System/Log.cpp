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

#include "..\..\Include\System\Log.h"
#include <assert.h>

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\Win32\Win32LogImpl.h"
#elif defined(RE_PLATFORM_ANDROID)
#include "..\..\Include\Android\AndroidLogImpl.h"
#else
#error RayEngine needs to have a platform defined
#endif

namespace RayEngine
{
	namespace System
	{
		/////////////////////////////////////////////////////////////
		Log::Log()
			: m_Impl(nullptr)
		{
#if defined(RE_PLATFORM_WINDOWS)
			m_Impl = new Win32LogImpl();
#elif defined(RE_PLATFORM_ANDROID)
			m_Impl = new AndroidLogImpl();
#endif
		}



		/////////////////////////////////////////////////////////////
		Log::~Log()
		{
			if (m_Impl != nullptr)
			{
				delete m_Impl;
				m_Impl = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		void Log::Write(LOG_SEVERITY severity, const Tchar* message, ...)
		{
			assert(m_Impl != nullptr);

			va_list args;
			va_start(args, message);
			Write(severity, message, args);
			va_end(args);
		}



		/////////////////////////////////////////////////////////////
		void Log::Write(LOG_SEVERITY severity, const Tchar* message, va_list args)
		{
			assert(m_Impl != nullptr);
			m_Impl->Write(severity, message, args);

			m_Messages.push_back({ severity, message });
		}



		/////////////////////////////////////////////////////////////
		void Log::Write(LOG_SEVERITY severity, const std::string& text)
		{
			assert(m_Impl != nullptr);

			const Tchar* t = text.c_str();
			Write(severity, t);
		}



		/////////////////////////////////////////////////////////////
		void Log::Flush()
		{
			m_Messages.clear();
		}



		/////////////////////////////////////////////////////////////
		int32 Log::GetMessageCount() const
		{
			return static_cast<int32>(m_Messages.size());
		}



		/////////////////////////////////////////////////////////////
		const LogMessage& Log::GetMessage(int32 index) const
		{
			return m_Messages[index];
		}
	}
}