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

namespace RayEngine
{
	class RAYENGINE_API TimeStamp
	{
	public:
		TimeStamp(int64 nanoseconds = 0);
		TimeStamp(TimeStamp&& other);
		TimeStamp(const TimeStamp& other);

		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			Adds seconds to the timestamp.

			seconds - Amount of seconds

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		void AddSeconds(double seconds);
		
		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			Adds milliseconds to the timestamp.

			milliseconds - Amount of milliseconds

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		void AddMilliSeconds(double milliseconds);
				
		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			Adds microseconds to the timestamp.

			microseconds - Amount of microseconds

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		void AddMicroSeconds(double microseconds);
		
		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			Subtracts seconds to the timestamp.

			seconds - Amount of seconds

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		void SubSeconds(double seconds);
		
		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			Subtracts milliseconds to the timestamp.

			milliseconds - Amount of milliseconds

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		void SubMilliSeconds(double milliseconds);
				
		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			Subtracts microseconds to the timestamp.

			microseconds - Amount of microseconds

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		void SubMicroSeconds(double microseconds);
				
		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			Sets the timestamp value to the specifed amount in seconds.

			seconds - Amount of seconds

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		void SetAsSeconds(double seconds);
		
		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			Sets the timestamp value to the specifed amount in milliseconds.

			milliseconds - Amount of milliseconds

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		void SetAsMilliSeconds(double milliseconds);
		
		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			Sets the timestamp value to the specifed amount in microseconds.

			microseconds - Amount of microseconds

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		void SetAsMicroSeconds(double microseconds);
		
		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			Sets the timestamp value to the specifed amount in nanoseconds.

			nanoseconds - Amount of nanoseconds

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		void SetAsNanoSeconds(double nanoseconds);

		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			Returns the timestamp as seconds
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		double GetAsSeconds() const;
		
		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			Returns the timestamp as milliSeconds
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		double GetAsMilliSeconds() const;
		
		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			Returns the timestamp as microSeconds
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		double GetAsMicroSeconds() const;
		
		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			Returns the timestamp as nanoSeconds
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
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