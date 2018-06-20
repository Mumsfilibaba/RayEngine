#include "..\..\Include\Android\AndroidLogImpl.h"

#if defined(RE_PLATFORM_ANDROID)
#include <android/log.h>

namespace RayEngine
{
	namespace System
	{
		/////////////////////////////////////////////////////////////
		AndroidLogImpl::AndroidLogImpl()
		{
		}



		/////////////////////////////////////////////////////////////
		AndroidLogImpl::~AndroidLogImpl()
		{
		}



		/////////////////////////////////////////////////////////////
		void AndroidLogImpl::Write(LOG_SEVERITY severity, const Tchar* text, va_list args) const
		{
			//Convert severity to priority
			int32 prio = 0;
			if (severity == LOG_SEVERITY_INFO)
				prio = ANDROID_LOG_INFO;
			else if (severity == LOG_SEVERITY_WARNING)
				prio = ANDROID_LOG_WARN;
			else if (severity == LOG_SEVERITY_ERROR)
				prio = ANDROID_LOG_ERROR;
			
			//Print to log
			__android_log_vprint(prio, "RayEngineLog", text, args);
		}
	}
}

#endif