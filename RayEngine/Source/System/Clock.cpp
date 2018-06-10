#include "..\..\Include\System\Clock.h"
#include <chrono>

namespace RayEngine
{
	Clock::Clock()
		: m_Delta(0),
		m_Total(0),
		m_LastTime(0)
	{
	}

	Clock::~Clock()
	{
	}

	void Clock::Tick()
	{
		using namespace std::chrono;

		//Get current time
		int64 now = time_point_cast<nanoseconds>(high_resolution_clock::now()).time_since_epoch().count();

		//Take the diff as delta
		m_Delta = TimeStamp(now - m_LastTime);
		m_Total += m_Delta;

		//Set now to lasttime
		m_LastTime = now;
	}

	void Clock::Reset()
	{
		m_Delta = TimeStamp(0);
		m_Total = TimeStamp(0);
	}

	const TimeStamp& Clock::GetDeltaTime() const
	{
		return m_Delta;
	}

	const TimeStamp& Clock::GetTotalTime() const
	{
		return m_Total;
	}
}