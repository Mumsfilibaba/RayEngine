#pragma once

#include "..\..\Include\System\Sensors.h"
#include "..\..\Include\Android\Android.h"

#if defined(RE_PLATFORM_ANDROID)

#include <android\sensor.h>

namespace RayEngine
{
	struct AndroidSensor
	{
		const ASensor* Sensor = nullptr;
		int32 Type = 0;
		SensorData Value;
		bool Enabled = false;
	};
}

#endif