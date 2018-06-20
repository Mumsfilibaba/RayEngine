#include "..\..\Include\System\Event.h"

namespace RayEngine
{
	namespace System
	{
		/////////////////////////////////////////////////////////////
		Event::Event()
		{
			memset(this, 0, sizeof(Event));
		}



		/////////////////////////////////////////////////////////////
		Event::Event(const Event& other)
		{
			memcpy(this, &other, sizeof(Event));
		}



		/////////////////////////////////////////////////////////////
		Event& Event::operator=(const Event& other)
		{
			memcpy(this, &other, sizeof(Event));
			return *this;
		}



		/////////////////////////////////////////////////////////////
		bool Event::operator==(const Event& other) const
		{
			return (Type == other.Type) && (TouchSize == other.TouchSize) &&
				(TouchFingerID == other.TouchFingerID) && (TouchPressure == other.TouchPressure) &&
				(TouchPosition == other.TouchPosition);
		}



		/////////////////////////////////////////////////////////////
		bool Event::operator!=(const Event& other) const
		{
			return !(*this == other);
		}
	}
}