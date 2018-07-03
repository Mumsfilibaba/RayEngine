#include "..\..\Include\System\TouchScreen.h"

#if defined(RE_PLATFORM_ANDROID)
#include "AndroidAppState.h"

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	bool TouchScreen::IsDown(int32 finger)
	{
		ALooper_pollAll(0, nullptr, nullptr, nullptr);

		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(GetNativeActivity()->instance);
		return state->FingerDown[finger];
	}



	/////////////////////////////////////////////////////////////
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