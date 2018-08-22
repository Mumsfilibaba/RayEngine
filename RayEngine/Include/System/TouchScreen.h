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
#include "..\Math\Vector2.h"

namespace RayEngine
{
	class TouchScreen
	{
	public:
		TouchScreen() = delete;
		~TouchScreen() = delete;
		TouchScreen(TouchScreen&& other) = delete;
		TouchScreen(const TouchScreen& other) = delete;
		TouchScreen& operator=(TouchScreen&& other) = delete;
		TouchScreen& operator=(const TouchScreen& other) = delete;

	public:
		static bool IsDown(int32 finger);
		static Math::Vector2 GetPosition(int32 finger);
	};
}