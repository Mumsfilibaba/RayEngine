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

#include <RayEngine.h>
#include <System/Clock.h>

#if defined(RE_PLATFORM_WINDOWS)

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
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Debug::Log(LOG_SEVERITY severity, const char* pText)
	{
		if (GetConsoleWindow() == 0)
		{
			AllocConsole();

			freopen("CONOUT$", "w", stdout);

			SetConsoleTitle(RE_T("RayEngine Log"));
		}

		WORD white = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		WORD color = white;

		std::string message;
		if (severity == LOG_SEVERITY_INFO)
		{
			color = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
			message = "[INFO]: ";
		}
		else if (severity == LOG_SEVERITY_WARNING)
		{
			color = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;
			message = "[WARNING]: ";
		}
		else if (severity == LOG_SEVERITY_ERROR)
		{
			color = FOREGROUND_RED;
			message = "[ERROR]: ";
		}

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
		std::string date = '[' + Clock::DateString() + "] ";
		std::string totalString = date + message + pText + '\n';
		print(totalString.c_str());
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), white);

		OutputDebugString(totalString.c_str());
	}
}

#endif