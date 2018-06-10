#pragma once

#include "..\Defines.h"
#include "..\Types.h"

#include <stdarg.h>

namespace RayEngine
{
	namespace System
	{
		enum LOG_SEVERITY 
		{
			LOG_SEVERITY_UNKNOWN = 0,
			LOG_SEVERITY_INFO = 1,
			LOG_SEVERITY_WARNING = 2,
			LOG_SEVERITY_ERROR = 3
		};

		//Message
		struct LogMessage
		{
			LOG_SEVERITY Severity;
			const Tchar* Message;
		};

		//Platform implementation of log
		class LogImpl
		{
		public:
			LogImpl(LogImpl&& other) = delete;
			LogImpl(const LogImpl& other) = delete;
			LogImpl& operator=(LogImpl&& other) = delete;
			LogImpl& operator=(const LogImpl& other) = delete;

			LogImpl() {}
			virtual ~LogImpl() {}

			//Log something to the platforms log (Console - Win32, LogCat - Android) - Also saves the log in a buffer
			virtual void Write(LOG_SEVERITY severity, const Tchar* text, va_list args) const = 0;
		};

		//Log-class
		class Log
		{
		public:
			Log(Log&& other) = delete;
			Log(const Log& other) = delete;
			Log& operator=(Log&& other) = delete;
			Log& operator=(const Log& other) = delete;
			
			Log();
			~Log();

			//Log something to the platforms log (Console - Win32, LogCat - Android) - Also saves the log in a buffer
			void Write(LOG_SEVERITY severity, const Tchar* text, ...) const;
			void Write(LOG_SEVERITY severity, const Tchar* text, va_list args) const;

		private:
			LogImpl* m_Impl;
		};
	}
}