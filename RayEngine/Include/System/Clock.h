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