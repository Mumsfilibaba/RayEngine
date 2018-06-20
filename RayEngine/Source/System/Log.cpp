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
		void Log::Write(LOG_SEVERITY severity, const Tchar* message, ...) const
		{
			assert(m_Impl != nullptr);

			va_list args;
			va_start(args, message);
			m_Impl->Write(severity, message, args);
			va_end(args);
		}



		/////////////////////////////////////////////////////////////
		void Log::Write(LOG_SEVERITY severity, const Tchar* message, va_list args) const
		{
			assert(m_Impl != nullptr);
			m_Impl->Write(severity, message, args);
		}
	}
}