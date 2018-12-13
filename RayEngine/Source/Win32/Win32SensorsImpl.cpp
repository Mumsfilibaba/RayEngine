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

#include <RayEngine.h>

#if defined(RE_PLATFORM_WINDOWS)
#include <System/Sensors.h>

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Sensors::SensorSupported(SENSOR_TYPE sensor)
	{
		//Not implemented on windows
		return false;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Sensors::SensorEnabled(SENSOR_TYPE sensor)
	{
		//Not implemented on windows
		return false;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Sensors::EnableSensor(SENSOR_TYPE sensor)
	{
		//Not implemented on windows
		return false;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Sensors::DisableSensor(SENSOR_TYPE sensor)
	{
		//Not implemented on windows
		return false;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Sensors::SetRefreshRate(SENSOR_TYPE sensor, const TimeStamp& time)
	{
		//Not implemented on windows
		return false;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SensorData Sensors::GetSensorValue(SENSOR_TYPE sensor)
	{
		return SensorData();
	}
}

#endif