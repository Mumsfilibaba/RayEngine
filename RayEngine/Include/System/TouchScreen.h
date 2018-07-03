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