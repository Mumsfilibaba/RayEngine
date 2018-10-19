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

#include "AndroidAppState.h"

#if defined(RE_PLATFORM_ANDROID)
#include "AndroidKeycodes.h"

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void AndroidAppState_SetWindowFlags(AndroidAppState* state, int32 flags)
	{
		ANativeActivity_setWindowFlags(state->NativeActivity, flags, flags ^ (flags | state->WindowFlags));
		state->WindowFlags = flags;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int AndroidAppState_SensorCallback(int fd, int events, void* data)
	{
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(GetNativeActivity()->instance);
		ASensorEvent event;
		if (ASensorEventQueue_getEvents(state->SensorEventQueue, &event, 1) > 0)
		{
			SENSOR_TYPE type = SENSOR_TYPE_UNKNOWN;
			SensorData sensorData;
			float res = 0.0f;

			switch (event.type)
			{
			case ASENSOR_TYPE_ACCELEROMETER:
				type = SENSOR_TYPE_ACCELEROMETER;

				sensorData.Accelerometer.x = event.acceleration.x;
				sensorData.Accelerometer.y = event.acceleration.y;
				sensorData.Accelerometer.z = event.acceleration.z;
				break;

			case ASENSOR_TYPE_GYROSCOPE:
				type = SENSOR_TYPE_GYROSCOPE;

				sensorData.Gyroscope.x = event.vector.x;
				sensorData.Gyroscope.y = event.vector.y;
				sensorData.Gyroscope.z = event.vector.z;
				break;

			case ASENSOR_TYPE_MAGNETIC_FIELD:
				type = SENSOR_TYPE_MAGNETIC_FIELD;

				sensorData.MagneticField.x = event.magnetic.x;
				sensorData.MagneticField.y = event.magnetic.y;
				sensorData.MagneticField.z = event.magnetic.z;
				break;
			}

			if (type != SENSOR_TYPE_UNKNOWN)
			{
				if (state->Sensors[type].Value != sensorData)
				{
					state->Sensors[type].Value = sensorData;

					System::Event sensorEvent;
					sensorEvent.Type = System::EVENT_TYPE_SENSORCHANGED;
					sensorEvent.Sensor.Type = type;
					sensorEvent.Sensor.Data = sensorData;

					state->EventMutex.lock();
					state->EventQueue.push(sensorEvent);
					state->EventMutex.unlock();
				}
			}
		}

		return 1;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void AndroidAppState_InitializeSensors(AndroidAppState* state)
	{
		int32 type[] =
		{
			ASENSOR_TYPE_ACCELEROMETER,
			ASENSOR_TYPE_GYROSCOPE,
			ASENSOR_TYPE_MAGNETIC_FIELD,
		};

		ASensorManager* manager = ASensorManager_getInstance();
		state->SensorManager = manager;
		state->SensorEventQueue = ASensorManager_createEventQueue(manager, state->Looper, 1, AndroidAppState_SensorCallback, state);

		for (int32 i = 0; i < RE_SENSOR_COUNT; i++)
		{
			state->Sensors[i].Sensor = ASensorManager_getDefaultSensor(manager, type[i]);
			state->Sensors[i].Type = type[i];
		}
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
			Event event;

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


						event.Type = EVENT_TYPE_TOUCHMOVE;
						event.Touch.FingerID = id;
						event.Touch.Pressure = pressure;
						event.Touch.Size = size;
						event.Touch.Position = position;
						event.Touch.Time = eventLasted;
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
					EVENT_TYPE type = (action & (AMOTION_EVENT_ACTION_DOWN | AMOTION_EVENT_ACTION_POINTER_DOWN)) ? 
						EVENT_TYPE_TOUCHPRESSED : EVENT_TYPE_TOUCHRELEASED;
					int32 index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
					int32 id = AMotionEvent_getPointerId(pEvent, index);					
					float pressure = AMotionEvent_getPressure(pEvent, index);
					float size = AMotionEvent_getSize(pEvent, index);
					Math::Vector2 position(AMotionEvent_getX(pEvent, index), AMotionEvent_getY(pEvent, index));


					event.Type = type;
					event.Touch.Pressure = pressure;
					event.Touch.Size = size;
					event.Touch.Position = position;
					event.Touch.FingerID = id;
					event.Touch.Time = eventLasted;


					pState->FingerDown[id % RE_TOUCH_POINTS] = (action & (AMOTION_EVENT_ACTION_DOWN | AMOTION_EVENT_ACTION_POINTER_DOWN));
					pState->ScreenPoints[id % RE_TOUCH_POINTS] = position;
				}

				break;
			}

			pState->EventQueue.push(event);
		}
		else if (type == AINPUT_EVENT_TYPE_KEY)
		{
			int32 action = AKeyEvent_getAction(pEvent);
			
			Event event;
			if (action == AKEY_EVENT_ACTION_DOWN)
				event.Type = EVENT_TYPE_KEYPRESSED;
			else if (action == AKEY_EVENT_ACTION_UP)
				event.Type = EVENT_TYPE_KEYRELEASED;
			else
				return;

			event.Key.KeyCode = AndroidToRe(AKeyEvent_getKeyCode(pEvent));
			event.Key.RepeatCount = AKeyEvent_getRepeatCount(pEvent);

			pState->EventQueue.push(event);
		}
	}
}

#endif