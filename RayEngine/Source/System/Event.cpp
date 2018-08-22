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