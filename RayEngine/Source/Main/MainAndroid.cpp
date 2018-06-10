#include "..\..\Include\Defines.h"

#if defined(RE_PLATFORM_ANDROID)

/////////////////////////////////////////////////////////////



//"Main thread" entrypoint - For multiplatform
extern int main(int args, char* argsv[]);



/////////////////////////////////////////////////////////////



#include <thread>
#include <mutex>
#include <queue>

#include <android\log.h>
#include <android\native_activity.h>
#include <android\native_window.h>
#include <android\window.h>
#include <android\input.h>
#include <android\looper.h>

#include "..\..\Include\Android\Android.h"



/////////////////////////////////////////////////////////////



//Struct for keeping global app variables
struct App
{
	//The activity pointer
	ANativeActivity* Activity = nullptr;

	//The current window
	ANativeWindow* CurrentWindow = nullptr;
	RayEngine::int32 WinWidth = 0;
	RayEngine::int32 WinHeight = 0;
	RayEngine::int32 WinColor = 0;
	bool HasFocus = false;
	
	//Input queue
	AInputQueue* InputQueue = nullptr;
	ALooper* Looper = nullptr;
	
	//Handle events
	std::queue<ANDROID_EVENT> Events;
} app;

//App instance mutex
std::mutex mutex;


/////////////////////////////////////////////////////////////



bool AndroidAppHasFocus()
{
	//Lock
	std::lock_guard<std::mutex> lock(mutex);

	//Return local variable
	return app.HasFocus;
}



/////////////////////////////////////////////////////////////



void AndroidSetNativeWindowColor(RayEngine::int32 color)
{
	using namespace RayEngine;

	//Lock
	std::lock_guard<std::mutex> lock(mutex);

	if (app.WinWidth == 0 || app.WinHeight == 0)
		return;

	app.WinColor = color;

	if (app.CurrentWindow != nullptr)
	{
		ARect rect = {};
		ANativeWindow_Buffer buffer = {};

		rect.bottom = app.WinHeight;
		rect.right = app.WinWidth;

		if (ANativeWindow_lock(app.CurrentWindow, &buffer, &rect) == 0)
		{
			if (ANativeWindow_getFormat(app.CurrentWindow) == WINDOW_FORMAT_RGBA_8888)
			{
				for (int32 i = (app.WinWidth * app.WinHeight) - 1; i >= 0; i--)
					static_cast<int32*>(buffer.bits)[i] = app.WinColor;
			}

			ANativeWindow_unlockAndPost(app.CurrentWindow);
		}
	}
}

void AndroidSetNativeWindowSize(RayEngine::int32 width, RayEngine::int32 height)
{
	using namespace RayEngine;

	if (width == 0 || height == 0)
		return;

	//Lock
	std::lock_guard<std::mutex> lock(mutex);

	app.WinWidth = width;
	app.WinHeight = height;

	if (app.CurrentWindow != nullptr)
	{
		int32 w = ANativeWindow_getWidth(app.CurrentWindow);
		int32 h = ANativeWindow_getHeight(app.CurrentWindow);

		if (w != width && h != height)
			ANativeWindow_setBuffersGeometry(app.CurrentWindow, app.WinWidth, app.WinHeight, 0);
	}
}

void AndroidSetNativeWindowFlags(RayEngine::uint32 addFlags, RayEngine::uint32 removeFlags)
{
	using namespace RayEngine;

	if (addFlags == 0 && removeFlags)
		return;

	//Lock
	std::lock_guard<std::mutex> lock(mutex);

	if (app.Activity != nullptr)
	{
		ANativeActivity_setWindowFlags(app.Activity, addFlags, removeFlags);
	}
}



/////////////////////////////////////////////////////////////



//recive_event
ANDROID_EVENT AndroidReciveEvent()
{	
	//Lock
	std::lock_guard<std::mutex> lock(mutex);

	//Get first event and pop threadsafe
	ANDROID_EVENT event = ANDROID_EVENT_UNKNOWN;

	if (!app.Events.empty())
	{
		event = app.Events.front();
		app.Events.pop();
	}

	//Return event
	return event;
}

void AndroidSendEvent(ANDROID_EVENT event)
{
	//Lock
	std::lock_guard<std::mutex> lock(mutex);

	//Push event threadsafe
	app.Events.push(event);
}



/////////////////////////////////////////////////////////////



//Callbacks
void onConfigurationChanged(ANativeActivity* activity)
{
	LOGI("onConfigurationChanged");
}

void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue)
{
	LOGI("onInputQueueCreated");

	//Lock
	std::lock_guard<std::mutex> lock(mutex);

	//Set app's inputqueue
	app.InputQueue = queue;
}

void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue)
{
	LOGI("onInputQueueDestroyed");

	//Lock
	std::lock_guard<std::mutex> lock(mutex);

	//Set inputqueue to nullptr;
	if (queue == app.InputQueue)
		app.InputQueue = nullptr;
}

void onLowMemory(ANativeActivity* activity)
{
	LOGI("onLowMemory");

	AndroidSendEvent(ANDROID_EVENT_LOW_MEMORY);
}

void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window)
{
	LOGI("onNativeWindowCreated");

	//Lock mutex
	mutex.lock();

	//Set window
	app.CurrentWindow = window;

	//Unlock mutex
	mutex.unlock();

	//Change color and size
	AndroidSetNativeWindowSize(app.WinWidth, app.WinHeight);
	AndroidSetNativeWindowColor(app.WinColor);

	//Push event to mainthread
	AndroidSendEvent(ANDROID_EVENT_WINDOW_CREATE);
}

void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window)
{
	LOGI("onNativeWindowDestroyed");

	//Lock mutex
	mutex.lock();

	//Set window to nullptr
	if (window == app.CurrentWindow)
		app.CurrentWindow = nullptr;

	//Unlock mutex
	mutex.unlock();

	AndroidSendEvent(ANDROID_EVENT_WINDOW_DESTROYED);
}

void onNativeWindowRedrawNeeded(ANativeActivity* activity, ANativeWindow* window)
{
	LOGI("onNativeWindowRedrawNeeded");

	AndroidSendEvent(ANDROID_EVENT_WINDOW_REPAINT);
}

void onNativeWindowResized(ANativeActivity* activity, ANativeWindow* window)
{
	LOGI("onNativeWindowResized");

	AndroidSendEvent(ANDROID_EVENT_WINDOW_RESIZE);
}

void onStart(ANativeActivity* activity)
{
	LOGI("onStart");

	AndroidSendEvent(ANDROID_EVENT_APP_START);
}

void onPause(ANativeActivity* activity)
{
	LOGI("onPause");

	AndroidSendEvent(ANDROID_EVENT_APP_PAUSE);
}

void onResume(ANativeActivity* activity)
{
	LOGI("onResume");

	AndroidSendEvent(ANDROID_EVENT_APP_RESUME);
}

void onStop(ANativeActivity* activity)
{
	LOGI("onStop");

	AndroidSendEvent(ANDROID_EVENT_APP_STOP);
}

void onDestroy(ANativeActivity* activity)
{
	LOGI("onDestroy");
	
	AndroidSendEvent(ANDROID_EVENT_DESTROY);
}

void* onSaveInstanceState(ANativeActivity* activity, size_t* outSize)
{
	LOGI("onSaveInstanceState");
	(*outSize) = 0;
	return nullptr;
}

void onWindowFocusChanged(ANativeActivity* activity, int hasFocus)
{
	LOGI("onWindowFocusChanged");

	//Lock mutex
	mutex.lock();

	//Set if the app has focus
	app.HasFocus = (hasFocus) ? true : false;
	
	//Lock mutex
	mutex.unlock();

	AndroidSendEvent(ANDROID_EVENT_FOCUS_CHANGED);
}

void onContentRectChanged(ANativeActivity* activity, const ARect* rect)
{
	LOGI("onContentRectChanged");
}



/////////////////////////////////////////////////////////////



void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize)
{
	LOGI("ANativeActivity_onCreate");

	//Set appinstance's activity
	app.Activity = activity;
	
	//Setup callbacks
	app.Activity->callbacks->onConfigurationChanged = onConfigurationChanged;
	app.Activity->callbacks->onInputQueueCreated = onInputQueueCreated;
	app.Activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;
	app.Activity->callbacks->onLowMemory = onLowMemory;
	app.Activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
	app.Activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
	app.Activity->callbacks->onNativeWindowRedrawNeeded = onNativeWindowRedrawNeeded;
	app.Activity->callbacks->onNativeWindowResized = onNativeWindowResized;
	app.Activity->callbacks->onStart = onStart;
	app.Activity->callbacks->onPause = onPause;
	app.Activity->callbacks->onResume = onResume;
	app.Activity->callbacks->onDestroy = onDestroy;
	app.Activity->callbacks->onStop = onStop;
	app.Activity->callbacks->onSaveInstanceState = onSaveInstanceState;
	app.Activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
	app.Activity->callbacks->onContentRectChanged = onContentRectChanged;

	//Start main in a seperate detached thread 
	std::thread mainThread(main, 0, nullptr);
	mainThread.detach();
	
	return;
}

/////////////////////////////////////////////////////////////

#endif