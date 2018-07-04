#pragma once

#include "..\Defines.h"
#include "..\Types.h"

namespace RayEngine
{
	class TimeStamp
	{
	public:
		TimeStamp(int64 nanoseconds = 0);

		void AddSeconds(double seconds);
		void AddMilliSeconds(double milliseconds);
		void AddMicroSeconds(double microseconds);

		void SubSeconds(double seconds);
		void SubMilliSeconds(double milliseconds);
		void SubMicroSeconds(double microseconds);

		void SetAsSeconds(double seconds);
		void SetAsMilliSeconds(double milliseconds);
		void SetAsMicroSeconds(double microseconds);
		void SetAsNanoSeconds(double nanoseconds);

		double GetAsSeconds() const;
		double GetAsMilliSeconds() const;
		double GetAsMicroSeconds() const;
		double GetAsNanoSeconds() const;

		TimeStamp& operator=(const TimeStamp& other);
		TimeStamp& operator+=(const TimeStamp& other);
		TimeStamp& operator-=(const TimeStamp& other);

		bool operator==(const TimeStamp& other) const;
		bool operator!=(const TimeStamp& other) const;

		friend TimeStamp operator-(TimeStamp left, const TimeStamp& right);
		friend TimeStamp operator+(TimeStamp left, const TimeStamp& right);

	private:
		int64 m_Nanoseconds;

	public:
		static TimeStamp Seconds(double seconds);
		static TimeStamp MilliSeconds(double milliseconds);
		static TimeStamp MicroSeconds(double microseconds);
	};
}