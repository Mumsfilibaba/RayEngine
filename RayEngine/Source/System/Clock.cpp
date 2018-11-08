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

#include "../../Include/System/Clock.h"
#include <chrono>
#include <ctime>

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Clock::Clock()
		: 
		m_Delta(0),
		m_Total(0),
		m_LastTime(0)
	{
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Clock::Clock(Clock&& other)
		: m_Delta(other.m_Delta),
		m_Total(other.m_Total),
		m_LastTime(other.m_LastTime)
	{
		other.m_Delta = 0;
		other.m_Total = 0;
		other.m_LastTime = 0;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Clock::Clock(const Clock& other)
		: m_Delta(other.m_Delta),
		m_Total(other.m_Total),
		m_LastTime(other.m_LastTime)
	{
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Clock::Tick()
	{
		auto highResNow = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now());
		auto value = std::chrono::duration_cast<std::chrono::nanoseconds>(highResNow.time_since_epoch());

		TimeStamp now = TimeStamp(value.count());
		m_Delta = now - m_LastTime;

		m_LastTime = now;
		m_Total += m_Delta;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Clock::Reset()
	{
		m_Delta = TimeStamp();
		m_Total = TimeStamp();
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const TimeStamp& Clock::GetDeltaTime() const
	{
		return m_Delta;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const TimeStamp& Clock::GetTotalTickTime() const
	{
		return m_Total;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::string Clock::ClockString()
	{
		using namespace std;

		time_t now = time(0);
		tm* localNow = localtime(&now);

		return string(to_string(localNow->tm_hour) + ":" +
			to_string(localNow->tm_min) + ":" + to_string(localNow->tm_sec));
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::string Clock::DateString()
	{
		using namespace std;

		time_t now = time(0);
		tm* localNow = localtime(&now);

		return string(to_string(localNow->tm_mon + 1) + '-' + to_string(localNow->tm_mday) + ' ' +
			to_string(localNow->tm_hour) + ":" + to_string(localNow->tm_min) + ':' + to_string(localNow->tm_sec));
	}
}