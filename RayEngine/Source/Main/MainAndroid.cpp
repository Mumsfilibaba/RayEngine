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

#include "..\..\Include\Defines.h"

#if defined(RE_PLATFORM_ANDROID)
#include "..\..\Include\Android\AndroidVulkanWrapper.h"
#include "..\Android\AndroidAppState.h"
#include <thread>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern int main(int args, char* argsv[]);



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void onConfigurationChanged(ANativeActivity* activity);
void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue);
void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue);
void onLowMemory(ANativeActivity* activity);
void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window);
void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window);
void onNativeWindowRedrawNeeded(ANativeActivity* activity, ANativeWindow* window);
void onNativeWindowResized(ANativeActivity* activity, ANativeWindow* window);
void onStart(ANativeActivity* activity);
void onPause(ANativeActivity* activity);
void onResume(ANativeActivity* activity);
void onStop(ANativeActivity* activity);
void onDestroy(ANativeActivity* activity);
void* onSaveInstanceState(ANativeActivity* activity, size_t* outSize);
void onWindowFocusChanged(ANativeActivity* activity, int hasFocus);
void onContentRectChanged(ANativeActivity* activity, const ARect* rect);


namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ANativeActivity* NativeActivity = nullptr;



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetNativeActivity(ANativeActivity* activity)
	{
		NativeActivity = activity;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ANativeActivity* GetNativeActivity()
	{
		return NativeActivity;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void onConfigurationChanged(ANativeActivity* activity)
{
	LOGI("onConfigurationChanged");
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue)
{
	using namespace RayEngine;

	LOGI("onInputQueueCreated");

	AndroidAppState* state = reinterpret_cast<AndroidAppState*>(activity->instance);
	state->InputQueue = queue;

	AInputQueue_attachLooper(state->InputQueue, state->Looper, 1, AndroidAppState_InputCallback, state);
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue)
{
	using namespace RayEngine;

	LOGI("onInputQueueDestroyed");

	AndroidAppState* state = reinterpret_cast<AndroidAppState*>(activity->instance);
	
	AInputQueue_detachLooper(state->InputQueue);
	state->InputQueue = nullptr;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void onLowMemory(ANativeActivity* activity)
{
	LOGI("onLowMemory");
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window)
{
	using namespace RayEngine;

	LOGI("onNativeWindowCreated");

	AndroidAppState* state = reinterpret_cast<AndroidAppState*>(activity->instance);
	state->WindowMutex.lock();
	state->NativeWindow = window;

	AndroidAppState_SetWindowFlags(state, state->WindowFlags);
	AndroidAppState_SetWindowSize(state, state->WindowWidth, state->WindowHeight);
	AndroidAppState_SetWindowColor(state, state->WindowColor);

	state->WindowMutex.unlock();
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window)
{
	using namespace RayEngine;

	LOGI("onNativeWindowDestroyed");

	AndroidAppState* state = reinterpret_cast<AndroidAppState*>(activity->instance);
	state->WindowMutex.lock();
	state->NativeWindow = nullptr;
	state->WindowMutex.unlock();
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void onNativeWindowRedrawNeeded(ANativeActivity* activity, ANativeWindow* window)
{
	LOGI("onNativeWindowRedrawNeeded");
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void onNativeWindowResized(ANativeActivity* activity, ANativeWindow* window)
{
	using namespace RayEngine;
	using namespace System;

	LOGI("onNativeWindowResized");

	Event event;
	event.Type = EVENT_TYPE_RESIZE;
	event.Resize.Width = ANativeWindow_getWidth(window);
	event.Resize.Height = ANativeWindow_getHeight(window);

	AndroidAppState* state = reinterpret_cast<AndroidAppState*>(activity->instance);
	state->EventMutex.lock();
	state->EventQueue.push(event);
	state->EventMutex.unlock();
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void onStart(ANativeActivity* activity)
{
	LOGI("onStart");
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void onPause(ANativeActivity* activity)
{
	using namespace RayEngine;
	using namespace System;

	LOGI("onPause");

	Event event;
	event.Type = EVENT_TYPE_FOCUSCHANGED;
	event.FocusChanged.HasFocus = false;

	AndroidAppState* state = reinterpret_cast<AndroidAppState*>(activity->instance);
	state->EventMutex.lock();
	state->EventQueue.push(event);
	state->EventMutex.unlock();
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void onResume(ANativeActivity* activity)
{
	using namespace RayEngine;
	using namespace System;

	LOGI("onResume");

	Event event;
	event.Type = EVENT_TYPE_FOCUSCHANGED;
	event.FocusChanged.HasFocus = true;

	AndroidAppState* state = reinterpret_cast<AndroidAppState*>(activity->instance);
	state->EventMutex.lock();
	state->EventQueue.push(event);
	state->EventMutex.unlock();
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void onStop(ANativeActivity* activity)
{
	LOGI("onStop");
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void onDestroy(ANativeActivity* activity)
{
	using namespace RayEngine;
	using namespace System;

	LOGI("onDestroy");

	Event event;
	event.Type = EVENT_TYPE_QUIT;
	event.Quit.ExitCode = 0;

	AndroidAppState* state = reinterpret_cast<AndroidAppState*>(activity->instance);
	state->EventMutex.lock();
	state->EventQueue.push(event);
	state->EventMutex.unlock();

	ASensorManager_destroyEventQueue(state->SensorManager, state->SensorEventQueue);
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void* onSaveInstanceState(ANativeActivity* activity, size_t* outSize)
{
	LOGI("onSaveInstanceState");
	(*outSize) = 0;
	return nullptr;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void onWindowFocusChanged(ANativeActivity* activity, int hasFocus)
{
	using namespace RayEngine;

	LOGI("onWindowFocusChanged");

	AndroidAppState* state = reinterpret_cast<AndroidAppState*>(activity->instance);
	state->HasFocus = (hasFocus != 0);
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void onContentRectChanged(ANativeActivity* activity, const ARect* rect)
{
	LOGI("onContentRectChanged");
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize)
{
	using namespace RayEngine;

	LOGI("ANativeActivity_onCreate");

	AndroidAppState* state = new AndroidAppState();
	state->NativeActivity = activity;
	
	
	JavaVM* vm = activity->vm;
	JNIEnv* env = nullptr;
	vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);

	jclass jActivity = env->GetObjectClass(activity->clazz);
	jmethodID jActivity_GetWindowManager = env->GetMethodID(jActivity, "getWindowManager", "()Landroid/view/WindowManager;");

	jclass jDisplayMetrics = env->FindClass("android/util/DisplayMetrics");
	jmethodID jDisplayMetrics_Init = env->GetMethodID(jDisplayMetrics, "<init>", "()V");
	jfieldID jDisplayMetrics_mWidthPixels = env->GetFieldID(jDisplayMetrics, "widthPixels", "I");
	jfieldID jDisplayMetrics_mHeightPixels = env->GetFieldID(jDisplayMetrics, "heightPixels", "I");

	jclass jDisplay = env->FindClass("android/view/Display");
	jmethodID jDisplay_GetMetrics = env->GetMethodID(jDisplay, "getMetrics", "(Landroid/util/DisplayMetrics;)V");

	jclass jWindowManager = env->FindClass("android/view/WindowManager");
	jmethodID jWindowManager_GetDefaultDisplay = env->GetMethodID(jWindowManager, "getDefaultDisplay", "()Landroid/view/Display;");

	jobject dm = env->NewObject(jDisplayMetrics, jDisplayMetrics_Init);
	jobject winM = env->CallObjectMethod(activity->clazz, jActivity_GetWindowManager);
	jobject display = env->CallObjectMethod(winM, jWindowManager_GetDefaultDisplay);
	env->CallVoidMethod(display, jDisplay_GetMetrics, dm);

	state->DisplayWidth = env->GetIntField(dm, jDisplayMetrics_mWidthPixels);
	state->DisplayHeight = env->GetIntField(dm, jDisplayMetrics_mHeightPixels);


	state->Assetmanager = activity->assetManager;
	AConfiguration* configuration = AConfiguration_new();
	AConfiguration_fromAssetManager(configuration, activity->assetManager);

	state->VersionSDK = AConfiguration_getSdkVersion(configuration);
	state->Configuration = configuration;


	state->Looper = ALooper_forThread();
	if (state->Looper == nullptr)
		state->Looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);


	AndroidAppState_InitializeSensors(state);


	activity->callbacks->onConfigurationChanged = onConfigurationChanged;
	activity->callbacks->onInputQueueCreated = onInputQueueCreated;
	activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;
	activity->callbacks->onLowMemory = onLowMemory;
	activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
	activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
	activity->callbacks->onNativeWindowRedrawNeeded = onNativeWindowRedrawNeeded;
	activity->callbacks->onNativeWindowResized = onNativeWindowResized;
	activity->callbacks->onStart = onStart;
	activity->callbacks->onPause = onPause;
	activity->callbacks->onResume = onResume;
	activity->callbacks->onDestroy = onDestroy;
	activity->callbacks->onStop = onStop;
	activity->callbacks->onSaveInstanceState = onSaveInstanceState;
	activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
	activity->callbacks->onContentRectChanged = onContentRectChanged;


	activity->instance = reinterpret_cast<void*>(state);

	SetNativeActivity(activity);


	if (!InitializeVulkan())
	{
		LOGE("Could not initialize Vulkan API, maybe not supported");
	}

	std::thread mainThread(main, 0, nullptr);
	mainThread.detach();
	
	return;
}
#endif