#pragma once

#include "..\System\Log.h"

#if defined(RE_PLATFORM_ANDROID)

namespace RayEngine
{
	namespace System
	{
		class AndroidLogImpl : public LogImpl
		{
		public:
			AndroidLogImpl(LogImpl&& other) = delete;
			AndroidLogImpl(const LogImpl& other) = delete;
			AndroidLogImpl& operator=(LogImpl&& other) = delete;
			AndroidLogImpl& operator=(const LogImpl& other) = delete;

			AndroidLogImpl();
			~AndroidLogImpl();

			//Log something to the platforms log (Console - Win32, LogCat - Android) - Also saves the log in a buffer
			void Write(LOG_SEVERITY severity, const Tchar* text, va_list args) const override final;
		};
	}
}

#endif