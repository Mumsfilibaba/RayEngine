#include "..\..\Include\System\Event.h"

namespace RayEngine
{
	namespace System
	{
		Event::Event(EVENT_TYPE type, int64 value)
			: type(type),
			value(value)
		{
		}

		bool Event::operator==(const Event& other) const
		{
			return (type == other.type) && (value == other.value);
		}

		bool Event::operator!=(const Event& other) const
		{
			return !(*this == other);
		}
	}
}