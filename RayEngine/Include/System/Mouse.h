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
#include "../System/KeyCodes.h"
#include "../System/Window.h"
#include "../Math/Point.h"

namespace RayEngine
{
	class Mouse
	{
		RE_STATIC_CLASS(Mouse);

	public:
		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			Sets the position of the mousecursor to the specified position in global space.

			position - The new position of the cursor.

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		static void SetPosition(const Math::Point& position);

		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			Sets the position of the mousecursor to the specified position relative to the specified window.

			position - The new position of the cursor.

			pWindow - The window to set the position in.

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		static void SetPosition(const Math::Point& position, const IWindow* pWindow);
		
		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			Returns the position of the mousecursor to the specified position in global space.
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		static Math::Point GetPosition();
		
		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			Returns the position of the mousecursor to the specified position relative to the specified window.

			pWindow - The window the cursor is relative to.

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		static Math::Point GetPosition(const IWindow* pWindow);

		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			Returns true if a mousebutton is currently pressed down.

			button - KeyCode to the button to check the state for.

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		static bool IsButtonDown(MOUSEBUTTON button);
	};
}