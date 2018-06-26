#include "..\..\Include\System\Clock.h"
#include <chrono>
#include <ctime>

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	Clock::Clock()
		: m_Delta(0),
		m_Total(0),
		m_LastTime(0)
	{
	}



	/////////////////////////////////////////////////////////////
	void Clock::Tick()
	{
		auto highResNow = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now());
		auto value = std::chrono::duration_cast<std::chrono::nanoseconds>(highResNow.time_since_epoch());

		TimeStamp now = TimeStamp(value.count());
		m_Delta = now - m_LastTime;

		m_LastTime = now;
		m_Total += m_Delta;
	}



	/////////////////////////////////////////////////////////////
	void Clock::Reset()
	{
		m_Delta = TimeStamp();
		m_Total = TimeStamp();
	}



	/////////////////////////////////////////////////////////////
	const TimeStamp& Clock::GetDeltaTime() const
	{
		return m_Delta;
	}



	/////////////////////////////////////////////////////////////
	const TimeStamp& Clock::GetTotalTickTime() const
	{
		return m_Total;
	}



	/////////////////////////////////////////////////////////////
	std::string Clock::ClockString()
	{
		using namespace std;

		time_t now = time(0);
		tm* localNow = localtime(&now);

		return string(to_string(localNow->tm_hour) + ":" +
			to_string(localNow->tm_min) + ":" + to_string(localNow->tm_sec));
	}



	/////////////////////////////////////////////////////////////
	std::string Clock::DateString()
	{
		using namespace std;

		time_t now = time(0);
		tm* localNow = localtime(&now);

		return string(to_string(localNow->tm_mon + 1) + '-' + to_string(localNow->tm_mday) + ' ' +
			to_string(localNow->tm_hour) + ":" + to_string(localNow->tm_min) + ':' + to_string(localNow->tm_sec));
	}
}