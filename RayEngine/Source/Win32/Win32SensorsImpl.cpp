#include "..\..\Include\System\Sensors.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	bool Sensors::SensorSupported(SENSOR_TYPE sensor)
	{
		//Not implemented on windows
		return false;
	}



	/////////////////////////////////////////////////////////////
	bool Sensors::SensorEnabled(SENSOR_TYPE sensor)
	{
		//Not implemented on windows
		return false;
	}



	/////////////////////////////////////////////////////////////
	bool Sensors::EnableSensor(SENSOR_TYPE sensor)
	{
		//Not implemented on windows
		return false;
	}



	/////////////////////////////////////////////////////////////
	bool Sensors::DisableSensor(SENSOR_TYPE sensor)
	{
		//Not implemented on windows
		return false;
	}



	/////////////////////////////////////////////////////////////
	bool Sensors::SetRefreshRate(SENSOR_TYPE sensor, const TimeStamp& time)
	{
		//Not implemented on windows
		return false;
	}



	/////////////////////////////////////////////////////////////
	SensorData Sensors::GetSensorValue(SENSOR_TYPE sensor)
	{
		return SensorData();
	}
}

#endif