#pragma once

#include "..\System\Log.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace System
	{
		class RE_API Win32LogImpl : public ILogImpl
		{
		public:
			Win32LogImpl(Win32LogImpl&& other) = delete;
			Win32LogImpl(const Win32LogImpl& other) = delete;
			Win32LogImpl& operator=(Win32LogImpl&& other) = delete;
			Win32LogImpl& operator=(const Win32LogImpl& other) = delete;

			Win32LogImpl();
			~Win32LogImpl();

			void Write(LOG_SEVERITY severity, const Tchar* text, va_list args) const;
		};
	}
}

#endif