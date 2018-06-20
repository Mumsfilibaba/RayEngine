#include "..\..\Include\Defines.h"

#if defined(RE_PLATFORM_ANDROID)

/////////////////////////////////////////////////////////////
extern int main(int args, char* argsv[]);



/////////////////////////////////////////////////////////////
#include <thread>
#include "..\Android\AndroidAppState.h"
#include "..\Android\AndroidWindowManager.h"
#include "..\Android\AndroidEventManager.h"



/////////////////////////////////////////////////////////////
//Callbacks
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



/////////////////////////////////////////////////////////////
//Struct for keeping global app variables
RayEngine::AndroidAppState appState;




/////////////////////////////////////////////////////////////
bool AndroidAppHasFocus()
{
	return appState.HasFocus();
}



/////////////////////////////////////////////////////////////
void AndroidSetNativeWindowColor(RayEngine::int32 color)
{
	appState.GetWindow().SetColor(color);
}



/////////////////////////////////////////////////////////////
void AndroidSetNativeWindowSize(RayEngine::int32 width, RayEngine::int32 height)
{
	appState.GetWindow().SetSize(width, height);
}



/////////////////////////////////////////////////////////////
void AndroidSetNativeWindowFlags(RayEngine::uint32 flags)
{
	using namespace RayEngine;

	appState.GetWindow().SetFlags(appState.GetActivity(), flags);
}



/////////////////////////////////////////////////////////////
RayEngine::System::Event AndroidReciveEvent()
{	
	return appState.GetEvents().PopEvent();
}



/////////////////////////////////////////////////////////////
void AndroidSendEvent(const RayEngine::System::Event& pEvent)
{
	appState.GetEvents().PushEvent(pEvent);
}



/////////////////////////////////////////////////////////////
void onConfigurationChanged(ANativeActivity* activity)
{
	LOGI("onConfigurationChanged");
}



/////////////////////////////////////////////////////////////
void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue)
{
	LOGI("onInputQueueCreated");

	appState.GetEvents().OnInputQueueChanged(queue, false);
}



/////////////////////////////////////////////////////////////
void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue)
{
	LOGI("onInputQueueDestroyed");

	appState.GetEvents().OnInputQueueChanged(queue, true);
}



/////////////////////////////////////////////////////////////
void onLowMemory(ANativeActivity* activity)
{
	LOGI("onLowMemory");
}



/////////////////////////////////////////////////////////////
void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window)
{
	LOGI("onNativeWindowCreated");

	//Call callback
	appState.GetWindow().OnNativeWindowChanged(activity, window, false);
}



/////////////////////////////////////////////////////////////
void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window)
{
	LOGI("onNativeWindowDestroyed");

	//Call callback
	appState.GetWindow().OnNativeWindowChanged(activity, window, true);
}



/////////////////////////////////////////////////////////////
void onNativeWindowRedrawNeeded(ANativeActivity* activity, ANativeWindow* window)
{
	LOGI("onNativeWindowRedrawNeeded");
}



/////////////////////////////////////////////////////////////
void onNativeWindowResized(ANativeActivity* activity, ANativeWindow* window)
{
	using namespace RayEngine::System;

	LOGI("onNativeWindowResized");

	//TODO: Get the real size

	//Push resized event
	Event event;
	event.Type = EVENT_TYPE_RESIZE;
	event.Width = 100;
	event.Height = 100;

	appState.GetEvents().PushEvent(event);
}



/////////////////////////////////////////////////////////////
void onStart(ANativeActivity* activity)
{
	LOGI("onStart");
}



/////////////////////////////////////////////////////////////
void onPause(ANativeActivity* activity)
{
	using namespace RayEngine::System;

	LOGI("onPause");

	Event event;
	event.Type = EVENT_TYPE_APP_PAUSED;

	appState.GetEvents().PushEvent(event);
}



/////////////////////////////////////////////////////////////
void onResume(ANativeActivity* activity)
{
	using namespace RayEngine::System;

	LOGI("onResume");

	Event event;
	event.Type = EVENT_TYPE_APP_RESUMED;

	appState.GetEvents().PushEvent(event);
}



/////////////////////////////////////////////////////////////
void onStop(ANativeActivity* activity)
{
	LOGI("onStop");
}



/////////////////////////////////////////////////////////////
void onDestroy(ANativeActivity* activity)
{
	using namespace RayEngine::System;

	LOGI("onDestroy");

	Event event;
	event.Type = EVENT_TYPE_QUIT;
	event.QuitCode = 0;

	appState.GetEvents().PushEvent(event);
}



/////////////////////////////////////////////////////////////
void* onSaveInstanceState(ANativeActivity* activity, size_t* outSize)
{
	LOGI("onSaveInstanceState");
	(*outSize) = 0;
	return nullptr;
}



/////////////////////////////////////////////////////////////
void onWindowFocusChanged(ANativeActivity* activity, int hasFocus)
{
	LOGI("onWindowFocusChanged");

	appState.OnFocusChanged(hasFocus);
}



/////////////////////////////////////////////////////////////
void onContentRectChanged(ANativeActivity* activity, const ARect* rect)
{
	LOGI("onContentRectChanged");
}



/////////////////////////////////////////////////////////////
void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize)
{
	LOGI("ANativeActivity_onCreate");

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

	appState.SetActivity(activity);

	appState.GetEvents().CreateLooper();

	std::thread mainThread(main, 0, nullptr);
	mainThread.detach();
	
	return;
}
#endif