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

#if defined(RE_PLATFORM_ANDROID)
#include "..\..\Include\System\Mouse.h"

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Mouse::SetPosition(const Math::Point& position)
	{
		//Not implemented on Android
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Mouse::SetPosition(const Math::Point& position, const System::Window& window)
	{
		//Not implemented on Android
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Math::Point Mouse::GetPosition()
	{
		//Not implemented on Android
		return Math::Point();
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Math::Point Mouse::GetPosition(const System::Window& window)
	{
		//Not implemented on Android
		return Math::Point();
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Mouse::IsButtonPressed(MOUSEBUTTON button)
	{
		//Not implemented on Android
		return false;
	}
}

#endif