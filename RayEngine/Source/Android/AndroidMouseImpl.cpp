#include "..\..\Include\System\Mouse.h"

#if defined(RE_PLATFORM_ANDROID)

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	void Mouse::SetPosition(const Math::Point& position)
	{
		//Not implemented on Android
	}



	/////////////////////////////////////////////////////////////
	void Mouse::SetPosition(const Math::Point& position, const System::Window& window)
	{
		//Not implemented on Android
	}



	/////////////////////////////////////////////////////////////
	Math::Point Mouse::GetPosition()
	{
		//Not implemented on Android
		return Math::Point();
	}



	/////////////////////////////////////////////////////////////
	Math::Point Mouse::GetPosition(const System::Window& window)
	{
		//Not implemented on Android
		return Math::Point();
	}



	/////////////////////////////////////////////////////////////
	bool Mouse::IsButtonPressed(MOUSEBUTTON button)
	{
		//Not implemented on Android
		return false;
	}
}

#endif