#include "..\..\Include\Defines.h"

#if defined(RE_PLATFORM_WINDOWS)

#ifndef WIN32_LEAN_AND_MEAN 1
#define WIN32_LEAN_AND_MEAN 1
#endif

#include <Windows.h>

extern int main(int args, char* argsv[]);

#if defined(UNICODE)

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	return main(0, nullptr);
}

#else

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	return main(0, nullptr);
}

#endif //UNICODE or ASCII

#endif //PLATFORM WINDOWS?