#pragma once

#include "..\Defines.h"
#include "..\Types.h"
#include <cstdarg>
#include <string>
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

		struct RE_API LogMessage
		{
			LOG_SEVERITY Severity;
			const Tchar* Message;
		};

		//Platform implementation of log
		class RE_API ILogImpl
		{
		public:
			ILogImpl(ILogImpl&& other) = delete;
			ILogImpl(const ILogImpl& other) = delete;
			ILogImpl& operator=(ILogImpl&& other) = delete;
			ILogImpl& operator=(const ILogImpl& other) = delete;

			ILogImpl() {}
			virtual ~ILogImpl() {}

			//Log something to the platforms log (Console - Win32, LogCat - Android) - Also saves the log in a buffer
			virtual void Write(LOG_SEVERITY severity, const Tchar* text, va_list args) const = 0;
		};

		class RE_API Log
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
			void Write(LOG_SEVERITY severity, const std::string& text);

			//Flush - Removes all messages from the message buffer
			void Flush();

			//Retrive messages
			int32 GetMessageCount() const;
			const LogMessage& GetMessage(int32 index) const;

		private:
			const ILogImpl* m_Impl;
			std::vector<LogMessage> m_Messages;
		};
	}
}