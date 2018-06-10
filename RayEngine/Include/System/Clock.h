#pragma once

#include "TimeStamp.h"

namespace RayEngine
{
	//Class for taking time 
	class Clock
	{
	public:
		Clock();
		~Clock();

		//Makes a tick, delta is the time between ticks
		void Tick();
		//Sets total- and deltatime to zero
		void Reset();

		//Gets delta timestamp
		const TimeStamp& GetDeltaTime() const;
		//Gets total timestamp
		const TimeStamp& GetTotalTime() const;

	private:
		int64 m_LastTime;
		TimeStamp m_Delta;
		TimeStamp m_Total;
	};
}