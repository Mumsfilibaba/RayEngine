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