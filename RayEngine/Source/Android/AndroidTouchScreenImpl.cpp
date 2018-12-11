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
#include "..\..\Include\System\TouchScreen.h"
#include "AndroidAppState.h"

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool TouchScreen::IsDown(int32 finger)
	{
		ALooper_pollAll(0, nullptr, nullptr, nullptr);

		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(GetNativeActivity()->instance);
		return state->FingerDown[finger];
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Math::Vector2 TouchScreen::GetPosition(int32 finger)
	{
		if (IsDown(finger))
		{

			ALooper_pollAll(0, nullptr, nullptr, nullptr);

			AndroidAppState* state = reinterpret_cast<AndroidAppState*>(GetNativeActivity()->instance);
			return state->ScreenPoints[finger];
		}

		return Math::Vector2(-1.0f, -1.0f);
	}
}

#endif