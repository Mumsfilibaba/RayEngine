#pragma once

#include "TimeStamp.h"
#include <string>

namespace RayEngine
{
	//Class for taking time 
	class Clock
	{
	public:
		Clock();

		//Makes a tick, delta is the time between ticks
		void Tick();
		//Sets total- and deltatime to zero
		void Reset();

		//Gets delta timestamp
		const TimeStamp& GetDeltaTime() const;
		//Gets total timestamp
		const TimeStamp& GetTotalTickTime() const;

	private:
		TimeStamp m_LastTime;
		TimeStamp m_Delta;
		TimeStamp m_Total;

	public:
		//Returns current systemclock timestamp as a string
		static std::string ClockString();
		//Returns current systemclock date as a string
		static std::string DateString();
	};
}