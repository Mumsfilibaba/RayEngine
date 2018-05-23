#include "..\..\Include\Defines.h"

#if defined(RE_PLATFORM_ANDROID)

extern int main(int args, char* argsv[]);

#include <thread>
#include <mutex>
#include <queue>

#include <android\log.h>
#include <android\native_activity.h>
#include "..\..\Include\Android\Android.h"

/* For debug builds, always enable the debug traces in this library */
#ifndef NDEBUG
#  define LOGV(...)  ((void)__android_log_print(ANDROID_LOG_VERBOSE, "threaded_app", __VA_ARGS__))
#else
#  define LOGV(...)  ((void)0)
#endif

/////////////////////////////////////////////////////////////
enum ANDROID_EVENT : RayEngine::int32
{
	ANDROID_EVENT_UNKNOWN = 0,
	ANDROID_EVENT_DESTROY = -1,
	ANDROID_EVENT_WINDOW_CREATE = 1,
	ANDROID_EVENT_WINDOW_DESTROYED = 2,
	ANDROID_EVENT_WINDOW_RESIZE = 3,
	ANDROID_EVENT_WINDOW_REPAINT = 4,
};

//struct for keeping global app variables
struct AppState
{
	std::mutex eventMutex;
	std::queue<ANDROID_EVENT> events;
} g_AppState;
/////////////////////////////////////////////////////////////

//GetMessage
int GetMessage()
{
	//Lock eventqueue
	g_AppState.eventMutex.lock();

	//Wait until event
	while (g_AppState.events.empty());

	//Get first event and pop
	int ev = g_AppState.events.front();
	g_AppState.events.pop();

	//unlock eventqueue
	g_AppState.eventMutex.unlock();

	//Return event
	return ev;
}

/////////////////////////////////////////////////////////////
//Callbacks
void onConfigurationChanged(ANativeActivity* activity)
{
}

void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue)
{
}

void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue)
{
}

void onLowMemory(ANativeActivity* activity)
{
}

void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window)
{
	g_AppState.events.push(ANDROID_EVENT_WINDOW_CREATE);
}

void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window)
{
	g_AppState.events.push(ANDROID_EVENT_WINDOW_DESTROYED);
}

void onNativeWindowRedrawNeeded(ANativeActivity* activity, ANativeWindow* window)
{
	g_AppState.events.push(ANDROID_EVENT_WINDOW_REPAINT);
}

void onNativeWindowResized(ANativeActivity* activity, ANativeWindow* window)
{
	g_AppState.events.push(ANDROID_EVENT_WINDOW_RESIZE);
}

void onStart(ANativeActivity* activity)
{
}

void onPause(ANativeActivity* activity)
{
}

void onResume(ANativeActivity* activity)
{
}

void onStop(ANativeActivity* activity)
{
}

void onDestroy(ANativeActivity* activity)
{
	g_AppState.events.push(ANDROID_EVENT_DESTROY);
}

void* onSaveInstanceState(ANativeActivity* activity, size_t* outSize)
{
	(*outSize) = 0;
	return nullptr;
}

void onWindowFocusChanged(ANativeActivity* activity, int hasFocus)
{
}

void onContentRectChanged(ANativeActivity* activity, const ARect* rect)
{
}
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize)
{
	//Setup callbacks
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

	//Start main in a seperate detached thread 
	std::thread mainThread(main, 0, nullptr);
	mainThread.detach();
}
/////////////////////////////////////////////////////////////

#endif