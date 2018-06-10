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
		//Reference counting for logs
		//Used to determine if the console should be freed or not
		int32 g_ConsoleRef = 0;

		Win32LogImpl::Win32LogImpl()
		{
			//Increse ref
			g_ConsoleRef++;
		}

		Win32LogImpl::~Win32LogImpl()
		{
			//Decrease ref
			g_ConsoleRef--;
			if (g_ConsoleRef < 1)
				FreeConsole();

		}

		void Win32LogImpl::Write(LOG_SEVERITY severity, const Tchar* text, va_list args) const
		{
			//If there are no console window - Create one
			if (GetConsoleWindow() == 0)
			{
				AllocConsole();

				freopen("CONOUT$", "w", stdout);

				//Set the title
				SetConsoleTitle(RE_T("RayEngine Log"));
			}

			//Set textcolor based on severity
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

			//Print
			print(sText);
			vprint(text, args);
		}
	}
}

#endif