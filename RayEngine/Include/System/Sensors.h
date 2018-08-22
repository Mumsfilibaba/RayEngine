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

#pragma once
#include "..\Defines.h"
#include "..\Types.h"
#include "..\System\TimeStamp.h"
#include "..\Math\Vector3.h"

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	enum SENSOR_TYPE : int32
	{
		SENSOR_TYPE_UNKNOWN = -1,
		SENSOR_TYPE_ACCELEROMETER = 0,
		SENSOR_TYPE_GYROSCOPE = 1,
		SENSOR_TYPE_MAGNETIC_FIELD = 2,
	};



	/////////////////////////////////////////////////////////////
	struct SensorData
	{
	public:
		SensorData();
		SensorData(const SensorData& other);
		SensorData& operator=(const SensorData& other);
		
		bool operator==(const SensorData& other) const;
		bool operator!=(const SensorData& other) const;

	public:
		union
		{
			Math::Vector3 Accelerometer;
			Math::Vector3 Gyroscope;
			Math::Vector3 MagneticField;
		};
	};



	/////////////////////////////////////////////////////////////
	class Sensors
	{
	public:
		Sensors() = delete;
		~Sensors() = delete;
		Sensors(Sensors&& other) = delete;
		Sensors(const Sensors& other) = delete;
		Sensors& operator=(Sensors&& other) = delete;
		Sensors& operator=(const Sensors& other) = delete;

	public:
		static bool SensorSupported(SENSOR_TYPE sensor);
		static bool SensorEnabled(SENSOR_TYPE sensor);
		
		static bool EnableSensor(SENSOR_TYPE sensor);
		static bool DisableSensor(SENSOR_TYPE sensor);

		static bool SetRefreshRate(SENSOR_TYPE sensor, const TimeStamp& time);
		
		static SensorData GetSensorValue(SENSOR_TYPE sensor);
	};
}