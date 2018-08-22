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

#include "..\..\Include\System\TouchScreen.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	bool TouchScreen::IsDown(int32 finger)
	{
		//Not implemented
		return false;
	}



	/////////////////////////////////////////////////////////////
	Math::Vector2 TouchScreen::GetPosition(int32 finger)
	{
		//Not implemented
		return Math::Vector2();
	}
}

#endif