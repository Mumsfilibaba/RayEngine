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
#include "RayEngine.h"
#include "TimeStamp.h"

namespace RayEngine
{
	class RAYENGINE_API Clock
	{
	public:
		Clock();
		Clock(Clock&& other);
		Clock(const Clock& other);

		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            Let the clock click. This updates the total and delta time of the clock. Deltatime is the time between ticks,
			and the total time is measured from the first tick to the current tick.
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		void Tick();

		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            Resets the clock so that both deltatime and totaltime is zero
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		void Reset();

		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            Returns the current deltatime
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		const TimeStamp& GetDeltaTime() const;

		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            Returns the total time
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		const TimeStamp& GetTotalTickTime() const;

	private:
		TimeStamp m_Delta;
		TimeStamp m_Total;
		TimeStamp m_LastTime;

	public:
		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            Returns the current system clock as a string - result 'hh:mm:ss'
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		static std::string ClockString();

		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            Returns the current system date as a string  - result 'mon-day hh:mm:ss'
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		static std::string DateString();
	};
}