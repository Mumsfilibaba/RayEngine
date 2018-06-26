#pragma once

#include "..\Defines.h"
#include "..\Types.h"

#include <cstdarg>
#include <vector>

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
			void Write(LOG_SEVERITY severity, const Tchar* text, ...);
			void Write(LOG_SEVERITY severity, const Tchar* text, va_list args);

			//Flush - Removes all messages from the message buffer
			void Flush();

			//Retrive messages
			int32 GetMessageCount() const;
			const LogMessage& GetMessage(int32 index) const;

		private:
			const LogImpl* m_Impl;
			std::vector<LogMessage> m_Messages;
		};
	}
}