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

#pragma once

#include "..\..\Include\Defines.h"

#if defined(RE_PLATFORM_ANDROID)

#include "..\..\Include\Types.h"
#include "..\..\Include\Android\Android.h"
#include "AndroidSensor.h"
#include <mutex>
#include <queue>
#include <thread>
#include <android\configuration.h>
#include <android\looper.h>

#define RE_SENSOR_COUNT 3
#define RE_TOUCH_POINTS 10

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct AndroidAppState
	{
		ANativeActivity* NativeActivity = nullptr;
		AConfiguration* Configuration = nullptr;
		
		ANativeWindow* NativeWindow = nullptr;
		int32 WindowWidth = 0;
		int32 WindowHeight = 0;
		int32 WindowFlags = 0;
		int32 WindowColor = (255 << 24) | (255 << 16) | (255 << 8) | 255;
		std::mutex WindowMutex;

		AInputQueue* InputQueue = nullptr;
		ALooper* Looper = nullptr;
		std::mutex EventMutex;
		std::queue<System::Event> EventQueue;

		AAssetManager* Assetmanager = nullptr;

		ASensorManager* SensorManager = nullptr;
		ASensorEventQueue* SensorEventQueue = nullptr;
		AndroidSensor Sensors[RE_SENSOR_COUNT];

		Math::Vector2 ScreenPoints[RE_TOUCH_POINTS];
		int32 DisplayWidth = 0;
		int32 DisplayHeight = 0;
		int32 VersionSDK = 0;
		bool FingerDown[RE_TOUCH_POINTS];
		bool HasFocus = false;
	};



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void AndroidAppState_SetWindowFlags(AndroidAppState* state, int32 flags);
	void AndroidAppState_SetWindowColor(AndroidAppState* state, int32 color);
	void AndroidAppState_SetWindowSize(AndroidAppState* state, int32 width, int32 height);
	void AndroidAppState_ProcessInputEvent(AndroidAppState* state, AInputEvent* event);
	int AndroidAppState_InputCallback(int fd, int events, void* data);
	int AndroidAppState_SensorCallback(int fd, int events, void* data);
	void AndroidAppState_InitializeSensors(AndroidAppState* state);
}

#endif