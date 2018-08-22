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

#include "..\..\Include\System\Sensors.h"

#if defined(RE_PLATFORM_ANDROID)
#include "AndroidAppState.h"

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	bool Sensors::SensorSupported(SENSOR_TYPE sensor)
	{
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(GetNativeActivity()->instance);
		return (state->Sensors[sensor].Sensor != nullptr);
	}



	/////////////////////////////////////////////////////////////
	bool Sensors::SensorEnabled(SENSOR_TYPE sensor)
	{
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(GetNativeActivity()->instance);
		return state->Sensors[sensor].Enabled;
	}



	/////////////////////////////////////////////////////////////
	bool Sensors::EnableSensor(SENSOR_TYPE sensor)
	{
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(GetNativeActivity()->instance);
		
		ASensorEventQueue* eventQueue = state->SensorEventQueue;
		const ASensor* nativeSensor = state->Sensors[sensor].Sensor;

		if (ASensorEventQueue_enableSensor(state->SensorEventQueue, nativeSensor) >= 0)
		{
			int32 delay = ASensor_getMinDelay(nativeSensor);
			if (delay > 0)
			{
				ASensorEventQueue_setEventRate(eventQueue, nativeSensor, delay);
			}

			state->Sensors[sensor].Enabled = true;
			return true;
		}

		return false;
	}



	/////////////////////////////////////////////////////////////
	bool Sensors::DisableSensor(SENSOR_TYPE sensor)
	{
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(GetNativeActivity()->instance);
		if (ASensorEventQueue_disableSensor(state->SensorEventQueue, state->Sensors[sensor].Sensor) > 0)
		{
			state->Sensors[sensor].Enabled = false;
			return true;
		}

		return false;
	}



	/////////////////////////////////////////////////////////////
	bool Sensors::SetRefreshRate(SENSOR_TYPE sensor, const TimeStamp& time)
	{
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(GetNativeActivity()->instance);
		if (state->Sensors[sensor].Enabled)
		{
			int32 integerTime = static_cast<int32>(time.GetAsMicroSeconds());
			int32 result = ASensorEventQueue_setEventRate(state->SensorEventQueue, state->Sensors[sensor].Sensor, integerTime);
			if (result < 0)
				return false;
		}
		else
		{
			return false;
		}

		return true;
	}



	/////////////////////////////////////////////////////////////
	SensorData Sensors::GetSensorValue(SENSOR_TYPE sensor)
	{
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(GetNativeActivity()->instance);
		return (state->Sensors[sensor].Enabled) ? state->Sensors[sensor].Value : SensorData();
	}
}

#endif