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

#include "RayEngine.h"
#if defined(RE_PLATFORM_WINDOWS)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern int main(int argc, char* argv[]);

#if defined(UNICODE)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	return main(0, nullptr);
}*/
#else
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	return main(__argc, __argv);
}*/
#endif //UNICODE or ASCII
#endif //PLATFORM WINDOWS