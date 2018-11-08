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
	class Clock
	{
	public:
		Clock();
		Clock(Clock&& other);
		Clock(const Clock& other);

		void Tick();

		void Reset();

		const TimeStamp& GetDeltaTime() const;

		const TimeStamp& GetTotalTickTime() const;

	private:
		TimeStamp m_Delta;
		TimeStamp m_Total;
		TimeStamp m_LastTime;

	public:
		static std::string ClockString();

		static std::string DateString();
	};
}