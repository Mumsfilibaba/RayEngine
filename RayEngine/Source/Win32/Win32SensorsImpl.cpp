#include "..\..\Include\System\Sensors.h"

#if defined(RE_PLATFORM_WIN32)

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
	Math::Vector3 Sensors::GetSensorValue(SENSOR_TYPE)
	{
		//Not implemented on windows
		return Math::Vector3();
	}
}

#endif