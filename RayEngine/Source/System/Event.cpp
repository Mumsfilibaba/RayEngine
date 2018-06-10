#include "..\..\Include\System\Event.h"

namespace RayEngine
{
	namespace System
	{
		Event::Event(EVENT_TYPE type, int64 value)
			: Type(type),
			Value(value)
		{
		}

		bool Event::operator==(const Event& other) const
		{
			return (Type == other.Type) && (Value == other.Value);
		}

		bool Event::operator!=(const Event& other) const
		{
			return !(*this == other);
		}
	}
}