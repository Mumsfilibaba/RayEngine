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