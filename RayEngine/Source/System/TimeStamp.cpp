#include "..\..\Include\System\TimeStamp.h"

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	TimeStamp::TimeStamp(int64 nanoseconds)
		: m_Nanoseconds(nanoseconds)
	{
	}



	/////////////////////////////////////////////////////////////
	void TimeStamp::AddSeconds(double seconds)
	{
		m_Nanoseconds += static_cast<int64>(seconds * 1000.0 * 1000.0 * 1000.0);
	}



	/////////////////////////////////////////////////////////////
	void TimeStamp::AddMilliSeconds(double milliseconds)
	{
		m_Nanoseconds += static_cast<int64>(milliseconds * 1000.0 * 1000.0);
	}



	/////////////////////////////////////////////////////////////
	void TimeStamp::AddMicroSeconds(double microseconds)
	{
		m_Nanoseconds += static_cast<int64>(microseconds * 1000.0 * 1000.0);
	}



	/////////////////////////////////////////////////////////////
	void TimeStamp::SubSeconds(double seconds)
	{
		m_Nanoseconds -= static_cast<int64>(seconds * 1000.0 * 1000.0 * 1000.0);
	}



	/////////////////////////////////////////////////////////////
	void TimeStamp::SubMilliSeconds(double milliseconds)
	{
		m_Nanoseconds -= static_cast<int64>(milliseconds * 1000.0 * 1000.0);
	}



	/////////////////////////////////////////////////////////////
	void TimeStamp::SubMicroSeconds(double microseconds)
	{
		m_Nanoseconds -= static_cast<int64>(microseconds * 1000.0 * 1000.0);
	}



	/////////////////////////////////////////////////////////////
	double TimeStamp::GetAsSeconds() const
	{
		return GetAsMilliSeconds() / 1000.0;
	}



	/////////////////////////////////////////////////////////////
	double TimeStamp::GetAsMilliSeconds() const
	{
		return GetAsMicroSeconds() / 1000.0;
	}



	/////////////////////////////////////////////////////////////
	double TimeStamp::GetAsMicroSeconds() const
	{
		return GetAsNanoSeconds() / 1000.0;
	}



	/////////////////////////////////////////////////////////////
	double TimeStamp::GetAsNanoSeconds() const
	{
		return static_cast<double>(m_Nanoseconds);
	}



	/////////////////////////////////////////////////////////////
	TimeStamp& TimeStamp::operator=(const TimeStamp& other)
	{
		if (this != &other)
			m_Nanoseconds = other.m_Nanoseconds;

		return *this;
	}



	/////////////////////////////////////////////////////////////
	TimeStamp& TimeStamp::operator+=(const TimeStamp& other)
	{
		m_Nanoseconds += other.m_Nanoseconds;
		return *this;
	}



	/////////////////////////////////////////////////////////////
	TimeStamp& TimeStamp::operator-=(const TimeStamp& other)
	{
		m_Nanoseconds -= other.m_Nanoseconds;
		return *this;
	}



	/////////////////////////////////////////////////////////////
	bool TimeStamp::operator==(const TimeStamp& other) const
	{
		return m_Nanoseconds == other.m_Nanoseconds;
	}



	/////////////////////////////////////////////////////////////
	bool TimeStamp::operator!=(const TimeStamp& other) const
	{
		return !(*this == other);
	}



	/////////////////////////////////////////////////////////////
	TimeStamp operator-(TimeStamp left, const TimeStamp& right)
	{
		return TimeStamp(left.m_Nanoseconds - right.m_Nanoseconds);
	}



	/////////////////////////////////////////////////////////////
	TimeStamp operator+(TimeStamp left, const TimeStamp& right)
	{
		return TimeStamp(left.m_Nanoseconds + right.m_Nanoseconds);
	}
}