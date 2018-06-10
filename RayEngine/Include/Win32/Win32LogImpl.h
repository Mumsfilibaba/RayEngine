#pragma once

#include "..\System\Log.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace System
	{
		class Win32LogImpl : public LogImpl
		{
		public:
			Win32LogImpl(Win32LogImpl&& other) = delete;
			Win32LogImpl(const Win32LogImpl& other) = delete;
			Win32LogImpl& operator=(Win32LogImpl&& other) = delete;
			Win32LogImpl& operator=(const Win32LogImpl& other) = delete;

			Win32LogImpl();
			~Win32LogImpl();

			//Log something to the platforms log (Console - Win32, LogCat - Android) - Also saves the log in a buffer
			void Write(LOG_SEVERITY severity, const Tchar* text, va_list args) const;

		private:
		};
	}
}

#endif