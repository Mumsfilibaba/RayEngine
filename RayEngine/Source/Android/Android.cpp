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

#include "..\..\Include\Android\Android.h"
#include "AndroidAppState.h"

#if defined(RE_PLATFORM_ANDROID)

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int32 GetAndroidColor(uint8 r, uint8 g, uint8 b, uint8 a)
	{
		return (a << 24) | (b << 16) | (g << 8) | r;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void AndroidGetDisplaySize(int32& width, int32& height)
	{
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(GetNativeActivity()->instance);

		width = state->DisplayWidth;
		height = state->DisplayHeight;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int32 AndroidGetSDKVersion()
	{
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(GetNativeActivity()->instance);
		return state->VersionSDK;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool AndroidAppHasFocus()
	{
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(GetNativeActivity()->instance);
		return state->HasFocus;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void AndroidSetNativeWindowColor(int32 color)
	{
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(GetNativeActivity()->instance);
		state->WindowMutex.lock();

		AndroidAppState_SetWindowColor(state, color);
		
		state->WindowMutex.unlock();
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void AndroidSetNativeWindowSize(int32 width, int32 height)
	{
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(GetNativeActivity()->instance);
		state->WindowMutex.lock();

		AndroidAppState_SetWindowSize(state, width, height);

		state->WindowMutex.unlock();
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void AndroidSetNativeWindowFlags(uint32 flags)
	{
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(GetNativeActivity()->instance);
		state->WindowMutex.lock();

		AndroidAppState_SetWindowFlags(state, flags);

		state->WindowMutex.unlock();
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	System::Event AndroidReciveEvent()
	{
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(GetNativeActivity()->instance);

		System::Event event;

		state->EventMutex.lock();
		if (!state->EventQueue.empty())
		{
			event = state->EventQueue.front();
			state->EventQueue.pop();
		}
		state->EventMutex.unlock();

		return event;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void AndroidSendEvent(const System::Event& pEvent)
	{
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(GetNativeActivity()->instance);
		state->EventMutex.lock();

		state->EventQueue.push(pEvent);

		state->EventMutex.unlock();
	}
}

#endif