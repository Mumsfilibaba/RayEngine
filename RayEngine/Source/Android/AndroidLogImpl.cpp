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

#include "..\..\Include\Android\AndroidLogImpl.h"

#if defined(RE_PLATFORM_ANDROID)
#include <android/log.h>

namespace RayEngine
{
	namespace System
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		AndroidLogImpl::AndroidLogImpl()
		{
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		AndroidLogImpl::~AndroidLogImpl()
		{
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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