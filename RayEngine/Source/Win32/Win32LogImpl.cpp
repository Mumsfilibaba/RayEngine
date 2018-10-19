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

#include "..\..\Include\System\Clock.h"
#include "..\..\Include\Win32\Win32LogImpl.h"

#if defined(RE_PLATFORM_WINDOWS)

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN 1
#endif

#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <cstdio>

#if defined(_UNICODE)
#define CRT_O_TEXT _O_WTEXT
#define print(...) wprintf(__VA_ARGS__)
#define vprint(...) vwprintf(__VA_ARGS__)
#else
#define CRT_O_TEXT _O_TEXT
#define print(...) printf(__VA_ARGS__)
#define vprint(...) vprintf(__VA_ARGS__)
#endif

namespace RayEngine
{
	namespace System
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int32 g_ConsoleRef = 0;



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Win32LogImpl::Win32LogImpl()
		{
			//Increse ref
			g_ConsoleRef++;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Win32LogImpl::~Win32LogImpl()
		{
			//Decrease ref
			g_ConsoleRef--;
			if (g_ConsoleRef < 1)
				FreeConsole();

		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void Win32LogImpl::Write(LOG_SEVERITY severity, const Tchar* text, va_list args) const
		{
			if (GetConsoleWindow() == 0)
			{
				AllocConsole();

				freopen("CONOUT$", "w", stdout);

				SetConsoleTitle(RE_T("RayEngine Log"));
			}

			WORD color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
			const Tchar* sText = nullptr;
			
			if (severity == LOG_SEVERITY_INFO)
			{
				color = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
				sText = RE_T("INFO: ");
			}
			else if (severity == LOG_SEVERITY_WARNING)
			{
				color = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;
				sText = RE_T("WARNING: ");
			}
			else if (severity == LOG_SEVERITY_ERROR)
			{
				color = FOREGROUND_RED;
				sText = RE_T("ERROR: ");
			}

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);

			std::string date = (Clock::DateString() + ": ");
			print(date.c_str());
			print(sText);
			vprint(text, args);
			print(RE_T("\n"));
		}
	}
}

#endif