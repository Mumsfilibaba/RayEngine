#include "..\..\Include\System\Event.h"

namespace RayEngine
{
	namespace System
	{
		Event::Event()
			: Type()
		{
			TouchSize = 0.0f;
			QuitCode = 0;
			TouchPressure = 0.0;
		}

		bool Event::operator==(const Event& other) const
		{
			return (Type == other.Type) && (TouchSize == other.TouchSize) &&
				(TouchFingerID == other.TouchFingerID) && (TouchPressure == other.TouchPressure) &&
				(TouchX == other.TouchX) && (TouchY == other.TouchY);
		}

		bool Event::operator!=(const Event& other) const
		{
			return !(*this == other);
		}
	}
}