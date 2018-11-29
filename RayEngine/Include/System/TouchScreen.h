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
#include "../Defines.h"
#include "../Types.h"
#include "../Math/Vector2.h"

namespace RayEngine
{
	class TouchScreen
	{
		RE_STATIC_CLASS(TouchScreen);

	public:
		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			Returns true if specified finger is down. The finger-index specified is used if multiple fingers are used at the
			same time. Most often specifing zero will do.

			finger - The index of the finger to check

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		static bool IsDown(int32 finger);

		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			Returns the current position of the specified finger. The finger-index specified is used if multiple fingers are 
			used at the same time. Most often specifing zero will do. A call to TouchScreen::IsDown should be done to make 
			sure that the returned position is valid.

			finger - The index of the finger to check

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		static Math::Vector2 GetPosition(int32 finger);
	};
}