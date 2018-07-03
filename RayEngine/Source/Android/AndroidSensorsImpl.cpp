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
		if (ASensorEventQueue_enableSensor(state->SensorEventQueue, state->Sensors[sensor].Sensor) > 0)
		{
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
	Math::Vector3 Sensors::GetSensorValue(SENSOR_TYPE sensor)
	{
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(GetNativeActivity()->instance);
		if (state->Sensors[sensor].Enabled)
			return state->Sensors[sensor].Value;

		return Math::Vector3();
	}
}


#endif