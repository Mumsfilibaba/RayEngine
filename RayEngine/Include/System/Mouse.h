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