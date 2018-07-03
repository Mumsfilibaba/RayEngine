#pragma once

#include "..\Defines.h"
#include "..\Types.h"
#include "..\System\KeyCodes.h"

namespace RayEngine
{
	class Keyboard
	{
	public:
		Keyboard() = delete;
		~Keyboard() = delete;
		Keyboard(Keyboard&& other) = delete;
		Keyboard(const Keyboard& other) = delete;
		Keyboard& operator=(Keyboard&& other) = delete;
		Keyboard& operator=(const Keyboard& other) = delete;

	public:
		static bool IsPressed(KEY keyCode);
		static bool ShowVirtualKeyboardVisible();
		static bool HideVirtualKeyboardVisible();
	};
}