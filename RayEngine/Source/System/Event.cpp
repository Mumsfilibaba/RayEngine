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
			return (Type == other.Type) && (Touch.Position == other.Touch.Position)
				&& (Touch.FingerID == other.Touch.FingerID) && (Touch.Pressure == other.Touch.Pressure)
				&& (Touch.Size == other.Touch.Size);
		}



		/////////////////////////////////////////////////////////////
		bool Event::operator!=(const Event& other) const
		{
			return !(*this == other);
		}
	}
}