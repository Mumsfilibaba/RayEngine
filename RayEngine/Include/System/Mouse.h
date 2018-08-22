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
#include "..\Defines.h"
#include "..\Types.h"
#include "..\System\KeyCodes.h"
#include "..\System\Window.h"
#include "..\Math\Point.h"

namespace RayEngine
{
	class Mouse
	{
	public:
		Mouse() = delete;
		~Mouse() = delete;
		Mouse(Mouse&& other) = delete;
		Mouse(const Mouse& other) = delete;
		Mouse& operator=(Mouse&& other) = delete;
		Mouse& operator=(const Mouse& other) = delete;

	public:
		static void SetPosition(const Math::Point& position);
		static void SetPosition(const Math::Point& position, const System::Window& window);
		static Math::Point GetPosition();
		static Math::Point GetPosition(const System::Window& window);

		static bool IsButtonPressed(MOUSEBUTTON button);
	};
}