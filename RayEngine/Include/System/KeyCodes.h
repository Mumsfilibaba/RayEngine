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

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum KEY : int32
	{
		KEY_UNKNOWN = 0,
		//Arrowkeys
		KEY_LEFT = 1,
		KEY_UP = 2,
		KEY_RIGHT = 3,
		KEY_DOWN = 4,
		//0-9
		KEY_0 = 5,
		KEY_1 = 6,
		KEY_2 = 7,
		KEY_3 = 8,
		KEY_4 = 9,
		KEY_5 = 10,
		KEY_6 = 11,
		KEY_7 = 12,
		KEY_8 = 13,
		KEY_9 = 14,
		//A-Z
		KEY_A = 15,
		KEY_B = 16,
		KEY_C = 17,
		KEY_D = 18,
		KEY_E = 19,
		KEY_F = 20,
		KEY_G = 21,
		KEY_H = 22,
		KEY_I = 23,
		KEY_J = 24,
		KEY_K = 25,
		KEY_L = 26,
		KEY_M = 27,
		KEY_N = 28,
		KEY_O = 29,
		KEY_P = 30,
		KEY_Q = 31,
		KEY_R = 32,
		KEY_S = 33,
		KEY_T = 34,
		KEY_U = 35,
		KEY_V = 36,
		KEY_W = 37,
		KEY_X = 38,
		KEY_Y = 39,
		KEY_Z = 40,
		//Numpad 0-9
		KEY_NUMPAD_0 = 41,
		KEY_NUMPAD_1 = 42,
		KEY_NUMPAD_2 = 43,
		KEY_NUMPAD_3 = 44,
		KEY_NUMPAD_4 = 45,
		KEY_NUMPAD_5 = 46,
		KEY_NUMPAD_6 = 47,
		KEY_NUMPAD_7 = 48,
		KEY_NUMPAD_8 = 49,
		KEY_NUMPAD_9 = 50,
		//F1-F24 
		KEY_F1 = 51,
		KEY_F2 = 52,
		KEY_F3 = 53,
		KEY_F4 = 54,
		KEY_F5 = 55,
		KEY_F6 = 56,
		KEY_F7 = 57,
		KEY_F8 = 58,
		KEY_F9 = 59,
		KEY_F10 = 60,
		KEY_F11 = 61,
		KEY_F12 = 62,
		KEY_F13 = 63,
		KEY_F14 = 64,
		KEY_F15 = 65,
		KEY_F16 = 66,
		KEY_F17 = 67,
		KEY_F18 = 68,
		KEY_F19 = 69,
		KEY_F20 = 70,
		KEY_F21 = 71,
		KEY_F22 = 72,
		KEY_F23 = 73,
		KEY_F24 = 74,
		//Special keys
		KEY_CTRL = 75,
		KEY_SHIFT = 76,
		KEY_LEFT_SHIFT = 77,
		KEY_LEFT_CTRL = 78,
		KEY_RIGHT_SHIFT = 79,
		KEY_RIGHT_CTRL = 80,
		KEY_ALT = 81,
		KEY_LEFT_ALT = 82,
		KEY_ALTGR = 83,
		KEY_ESCAPE = 84,
		//OS-Keys
		KEY_PAGE_UP = 85,
		KEY_PAGE_DOWN = 86,
		KEY_END = 87,
		KEY_HOME = 88,
		KEY_LEFT_CMD = 89,
		KEY_RIGHT_CMD = 90,
		KEY_APPS = 91,
		KEY_HELP = 92,
		KEY_PAUSE = 93,
		//Textmodifiers
		KEY_INSERT = 94,
		KEY_DELETE = 95,
		KEY_BACKSPACE = 96,
		KEY_TAB = 97,
		KEY_CLEAR = 98,
		KEY_ENTER = 99,
		KEY_SPACE = 100,
		KEY_COMMA = 101,
		KEY_PERIOD = 102,
		KEY_TILDE = 103,
		KEY_QUOTE = 104,
		KEY_SLASH = 105,
		KEY_BACKSLASH = 106,
		KEY_SEMI_COLON = 107,
		KEY_DECIMAL = 108,
		//Math
		KEY_MINUS = 109,
		KEY_PLUS = 110,
		KEY_DIVIDE = 111,
		KEY_MULTIPLY = 112,
		KEY_SUBTRACT = 113,
		KEY_ADD = 114,
		//Brackets
		KEY_LEFT_BRACKET = 115,
		KEY_RIGHT_BRACKET = 116,
		KEY_ANGLE_BRACKET = 117,
		//Toggle
		KEY_NUMLOCK = 118,
		KEY_CAPSLOCK = 119,
		KEY_SCROLLOCK = 120,
	};



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum MOUSEBUTTON : int32
	{
		MOUSEBUTTON_UNKNOWN = 0,
		MOUSEBUTTON_LEFT = 1,
		MOUSEBUTTON_MIDDLE = 2,
		MOUSEBUTTON_RIGHT = 3,
		MOUSEBUTTON_BACK = 4,
		MOUSEBUTTON_FORWARD = 5,
	};
}