#include "AndroidAppState.h"

#if defined(RE_PLATFORM_ANDROID)

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	void AndroidAppState_SetWindowFlags(AndroidAppState* state, int32 flags)
	{
		ANativeActivity_setWindowFlags(state->NativeActivity, flags, flags ^ (flags | state->WindowFlags));
		state->WindowFlags = flags;
	}



	/////////////////////////////////////////////////////////////
	void AndroidAppState_SetWindowColor(AndroidAppState* state, int32 color)
	{
		if (state->WindowWidth == 0 || state->WindowHeight == 0)
			return;

		state->WindowColor = color;

		if (state->NativeWindow == nullptr)
			return;

		ARect rect = {};
		ANativeWindow_Buffer buffer = {};

		rect.right = state->WindowWidth;
		rect.bottom = state->WindowHeight;

		if (ANativeWindow_lock(state->NativeWindow, &buffer, &rect) == 0)
		{
			if (ANativeWindow_getFormat(state->NativeWindow) == WINDOW_FORMAT_RGBA_8888)
			{
				for (int32 i = (state->WindowWidth * state->WindowHeight) - 1; i >= 0; i--)
					static_cast<int32*>(buffer.bits)[i] = state->WindowColor;
			}
			else
			{
				LOGE("'ANativeWindow_getFormat' returned unsupported format");
			}

			if (ANativeWindow_unlockAndPost(state->NativeWindow) != 0)
			{
				LOGE("'ANativeWindow_unlockAndPost' failed");
			}
		}
	}



	/////////////////////////////////////////////////////////////
	void AndroidAppState_SetWindowSize(AndroidAppState* state, int32 width, int32 height)
	{
		if (width == 0 || height == 0)
			return;

		state->WindowWidth = width;
		state->WindowHeight = height;

		if (state->NativeWindow == nullptr)
			return;

		if (ANativeWindow_setBuffersGeometry(state->NativeWindow, state->WindowWidth, state->WindowHeight, 0) != 0)
		{
			LOGE("'ANativeWindow_setBuffersGeometry' failed");
		}
	}



	/////////////////////////////////////////////////////////////
	int AndroidAppState_InputCallback(int fd, int events, void* data)
	{
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(data);
		if (events & ALOOPER_EVENT_INPUT)
		{
			AInputEvent* event = nullptr;
			if (AInputQueue_getEvent(state->InputQueue, &event) >= 0)
			{
				if (!AInputQueue_preDispatchEvent(state->InputQueue, event))
				{
					state->EventMutex.lock();

					AndroidAppState_ProcessInputEvent(state, event);

					state->EventMutex.unlock();

					AInputQueue_finishEvent(state->InputQueue, event, 1);
				}
			}
		}

		return 1;
	}



	/////////////////////////////////////////////////////////////
	void AndroidAppState_ProcessInputEvent(AndroidAppState* pState, AInputEvent* pEvent)
	{
		using namespace Math;
		using namespace System;

		int32 type = AInputEvent_getType(pEvent);
		int32 source = AInputEvent_getSource(pEvent);

		if (type == AINPUT_EVENT_TYPE_MOTION)
		{
			int32 action = AMotionEvent_getAction(pEvent);
			int32 pointerCount = AMotionEvent_getPointerCount(pEvent);
			int32 actionMask = action & AMOTION_EVENT_ACTION_MASK;
			int32 downTime = AMotionEvent_getDownTime(pEvent);
			int32 eventTime = AMotionEvent_getEventTime(pEvent);

			TimeStamp eventLasted(eventTime - downTime);

			switch (actionMask)
			{
			case AMOTION_EVENT_ACTION_MOVE:

				if (source & AINPUT_SOURCE_TOUCHSCREEN)
				{
					for (int32 i = 0; i < pointerCount; i++)
					{
						Math::Vector2 position(AMotionEvent_getX(pEvent, i), AMotionEvent_getY(pEvent, i));
						float pressure = AMotionEvent_getPressure(pEvent, i);
						float size = AMotionEvent_getSize(pEvent, i);
						int32 id = AMotionEvent_getPointerId(pEvent, i);


						if (pState->ScreenPoints[id % RE_TOUCH_POINTS] == position)
							continue;
						else
							pState->ScreenPoints[id % RE_TOUCH_POINTS] = position;


						Event event;
						event.Type = EVENT_TYPE_TOUCHMOVE;
						event.Touch.FingerID = id;
						event.Touch.Pressure = pressure;
						event.Touch.Size = size;
						event.Touch.Position = position;
						event.Touch.Time = eventLasted;

						pState->EventQueue.push(event);
					}
				}

				break;
			case AMOTION_EVENT_ACTION_POINTER_DOWN:
			case AMOTION_EVENT_ACTION_DOWN:
			case AMOTION_EVENT_ACTION_POINTER_UP:
			case AMOTION_EVENT_ACTION_UP:
			case AMOTION_EVENT_ACTION_CANCEL:
				if (source & AINPUT_SOURCE_TOUCHSCREEN)
				{
					int32 index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
					
					Math::Vector2 position(AMotionEvent_getX(pEvent, index), AMotionEvent_getY(pEvent, index));
					float pressure = AMotionEvent_getPressure(pEvent, index);
					float size = AMotionEvent_getSize(pEvent, index);
					int32 id = AMotionEvent_getPointerId(pEvent, index);

					EVENT_TYPE type = (action & (AMOTION_EVENT_ACTION_DOWN | AMOTION_EVENT_ACTION_POINTER_DOWN)) ? 
						EVENT_TYPE_TOUCHPRESSED : EVENT_TYPE_TOUCHRELEASED;

					Event event;
					event.Type = type;
					event.Touch.Pressure = pressure;
					event.Touch.Size = size;
					event.Touch.Position = position;
					event.Touch.FingerID = id;
					event.Touch.Time = eventLasted;

					pState->ScreenPoints[id % RE_TOUCH_POINTS] = position;
					pState->EventQueue.push(event);
				}

				break;
			}
		}
	}
}

#endif