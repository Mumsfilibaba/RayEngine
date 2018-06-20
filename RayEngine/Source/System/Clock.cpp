#include "..\..\Include\System\Clock.h"
#include <chrono>

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
	Clock::~Clock()
	{
	}



	/////////////////////////////////////////////////////////////
	void Clock::Tick()
	{
		auto now = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now());
		auto value = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
		
		m_Delta = TimeStamp(value.count() - m_LastTime);
		m_Total += m_Delta;

		m_LastTime = value.count();
	}



	/////////////////////////////////////////////////////////////
	void Clock::Reset()
	{
		m_Delta = TimeStamp(0);
		m_Total = TimeStamp(0);
	}



	/////////////////////////////////////////////////////////////
	const TimeStamp& Clock::GetDeltaTime() const
	{
		return m_Delta;
	}



	/////////////////////////////////////////////////////////////
	const TimeStamp& Clock::GetTotalTime() const
	{
		return m_Total;
	}
}