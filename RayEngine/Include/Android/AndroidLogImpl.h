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

#pragma once
#include "..\System\Log.h"

#if defined(RE_PLATFORM_ANDROID)

namespace RayEngine
{
	namespace System
	{
		class AndroidLogImpl : public ILogImpl
		{
		public:
			AndroidLogImpl(ILogImpl&& other) = delete;
			AndroidLogImpl(const ILogImpl& other) = delete;
			AndroidLogImpl& operator=(ILogImpl&& other) = delete;
			AndroidLogImpl& operator=(const ILogImpl& other) = delete;

		public:
			AndroidLogImpl();
			~AndroidLogImpl();

			//Log something to the platforms log (Console - Win32, LogCat - Android) - Also saves the log in a buffer
			void Write(LOG_SEVERITY severity, const Tchar* pText, va_list args) const override final;
		};
	}
}

#endif