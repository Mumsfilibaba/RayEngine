#include "..\..\Include\System\Sensors.h"

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	SensorData::SensorData()
		: Accelerometer()
	{
	}



	/////////////////////////////////////////////////////////////
	SensorData::SensorData(const SensorData& other)
	{
		*this = other;
	}



	/////////////////////////////////////////////////////////////
	SensorData& SensorData::operator=(const SensorData& other)
	{
		Accelerometer = other.Accelerometer;
		return *this;
	}



	/////////////////////////////////////////////////////////////
	bool SensorData::operator==(const SensorData& other) const
	{
		return (Accelerometer == other.Accelerometer);
	}



	/////////////////////////////////////////////////////////////
	bool SensorData::operator!=(const SensorData& other) const
	{
		return !(*this == other);
	}
}