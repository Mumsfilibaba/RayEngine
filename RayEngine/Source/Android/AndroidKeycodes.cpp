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

#include "RayEngine.h"

#if defined(RE_PLATFORM_ANDROID)
#include "AndroidKeycodes.h"
#include <android/input.h>

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	KEY AndroidToRe(int32 keyCode)
	{
		switch (keyCode)
		{
			//Arrow
		//case 0x25: return KEY_LEFT;
		//case 0x26: return KEY_UP;
		//case 0x27: return KEY_RIGHT;
		//case 0x28: return KEY_DOWN;
			//0-9
		case AKEYCODE_0: return KEY_0;
		case AKEYCODE_1: return KEY_1;
		case AKEYCODE_2: return KEY_2;
		case AKEYCODE_3: return KEY_3;
		case AKEYCODE_4: return KEY_4;
		case AKEYCODE_5: return KEY_5;
		case AKEYCODE_6: return KEY_6;
		case AKEYCODE_7: return KEY_7;
		case AKEYCODE_8: return KEY_8;
		case AKEYCODE_9: return KEY_9;
			//A-Z
		case AKEYCODE_A: return KEY_A;
		case AKEYCODE_B: return KEY_B;
		case AKEYCODE_C: return KEY_C;
		case AKEYCODE_D: return KEY_D;
		case AKEYCODE_E: return KEY_E;
		case AKEYCODE_F: return KEY_F;
		case AKEYCODE_G: return KEY_G;
		case AKEYCODE_H: return KEY_H;
		case AKEYCODE_I: return KEY_I;
		case AKEYCODE_J: return KEY_J;
		case AKEYCODE_K: return KEY_K;
		case AKEYCODE_L: return KEY_L;
		case AKEYCODE_M: return KEY_M;
		case AKEYCODE_N: return KEY_N;
		case AKEYCODE_O: return KEY_O;
		case AKEYCODE_P: return KEY_P;
		case AKEYCODE_Q: return KEY_Q;
		case AKEYCODE_R: return KEY_R;
		case AKEYCODE_S: return KEY_S;
		case AKEYCODE_T: return KEY_T;
		case AKEYCODE_U: return KEY_U;
		case AKEYCODE_V: return KEY_V;
		case AKEYCODE_W: return KEY_W;
		case AKEYCODE_X: return KEY_X;
		case AKEYCODE_Y: return KEY_Y;
		case AKEYCODE_Z: return KEY_Z;
			//Numpad 0
		case AKEYCODE_NUMPAD_0: return KEY_NUMPAD_0;
		case AKEYCODE_NUMPAD_1: return KEY_NUMPAD_1;
		case AKEYCODE_NUMPAD_2: return KEY_NUMPAD_2;
		case AKEYCODE_NUMPAD_3: return KEY_NUMPAD_3;
		case AKEYCODE_NUMPAD_4: return KEY_NUMPAD_4;
		case AKEYCODE_NUMPAD_5: return KEY_NUMPAD_5;
		case AKEYCODE_NUMPAD_6: return KEY_NUMPAD_6;
		case AKEYCODE_NUMPAD_7: return KEY_NUMPAD_7;
		case AKEYCODE_NUMPAD_8: return KEY_NUMPAD_8;
		case AKEYCODE_NUMPAD_9: return KEY_NUMPAD_9;
			//F1-F12
		case AKEYCODE_F1: return KEY_F1;
		case AKEYCODE_F2: return KEY_F2;
		case AKEYCODE_F3: return KEY_F3;
		case AKEYCODE_F4: return KEY_F4;
		case AKEYCODE_F5: return KEY_F5;
		case AKEYCODE_F6: return KEY_F6;
		case AKEYCODE_F7: return KEY_F7;
		case AKEYCODE_F8: return KEY_F8;
		case AKEYCODE_F9: return KEY_F9;
		case AKEYCODE_F10: return KEY_F10;
		case AKEYCODE_F11: return KEY_F11;
		case AKEYCODE_F12: return KEY_F12;
			//Special
		//case 0x11: return KEY_CTRL;
		//case 0x10: return KEY_SHIFT;
		case AKEYCODE_SHIFT_LEFT: return KEY_LEFT_SHIFT;
		case AKEYCODE_CTRL_LEFT: return KEY_LEFT_CTRL;
		case AKEYCODE_SHIFT_RIGHT: return KEY_RIGHT_SHIFT;
		case AKEYCODE_CTRL_RIGHT: return KEY_RIGHT_CTRL;
		//case 0x12: return KEY_ALT;
		case AKEYCODE_ALT_LEFT: return KEY_LEFT_ALT;
		//case 0xA5: return KEY_ALTGR;
		case AKEYCODE_ESCAPE: return KEY_ESCAPE;
			//OS-Keys
		case AKEYCODE_PAGE_UP: return KEY_PAGE_UP;
		case AKEYCODE_PAGE_DOWN: return KEY_PAGE_DOWN;
		//case 0x23: return KEY_END;
		case AKEYCODE_HOME: return KEY_HOME;
		//case 0x5B: return KEY_LEFT_CMD;
		//case 0x5C: return KEY_RIGHT_CMD;
		//case 0x5D: return KEY_APPS;
		case AKEYCODE_HELP: return KEY_HELP;
		case AKEYCODE_BREAK: return KEY_PAUSE;
			//Textmodifiers
		case AKEYCODE_INSERT: return KEY_INSERT;
		case AKEYCODE_DEL: return KEY_DELETE;
		case AKEYCODE_FORWARD_DEL: return KEY_BACKSPACE;
		case AKEYCODE_TAB: return KEY_TAB;
		case AKEYCODE_CLEAR: return KEY_CLEAR;
		case AKEYCODE_ENTER: return KEY_ENTER;
		case AKEYCODE_SPACE: return KEY_SPACE;
		case AKEYCODE_COMMA: return KEY_COMMA;
		case AKEYCODE_PERIOD: return KEY_PERIOD;
		//case : return KEY_TILDE;
		//case	: return KEY_QUOTE;
		case AKEYCODE_SLASH: return KEY_SLASH;
		case AKEYCODE_BACKSLASH: return KEY_BACKSLASH;
		case AKEYCODE_SEMICOLON: return KEY_SEMI_COLON;
		//case AKEYCODE: return KEY_DECIMAL;
			//Math
		case AKEYCODE_MINUS: return KEY_MINUS;
		case AKEYCODE_PLUS: return KEY_PLUS;
		//case AKEYCODE_SLASH: return KEY_DIVIDE;
		case AKEYCODE_STAR: return KEY_MULTIPLY;
		//case 0x6D: return KEY_SUBTRACT;
		//case 0x6B: return KEY_ADD;
			//Brackets
		case AKEYCODE_LEFT_BRACKET: return KEY_LEFT_BRACKET;
		case AKEYCODE_RIGHT_BRACKET: return KEY_RIGHT_BRACKET;
		//case : return KEY_ANGLE_BRACKET;
			//Toggle
		case AKEYCODE_NUM_LOCK: return KEY_NUMLOCK;
		case AKEYCODE_CAPS_LOCK: return KEY_CAPSLOCK;
		case AKEYCODE_SCROLL_LOCK: return KEY_SCROLLOCK;
		default: return KEY_UNKNOWN;
		}
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int32 ReToAndroid(KEY keyCode)
	{
		switch (keyCode)
		{
			//0-9
		case KEY_0: return AKEYCODE_0;
		case KEY_1: return AKEYCODE_1;
		case KEY_2: return AKEYCODE_2;
		case KEY_3: return AKEYCODE_3;
		case KEY_4: return AKEYCODE_4;
		case KEY_5: return AKEYCODE_5;
		case KEY_6: return AKEYCODE_6;
		case KEY_7: return AKEYCODE_7;
		case KEY_8: return AKEYCODE_8;
		case KEY_9: return AKEYCODE_9;
			//A-Z
		case KEY_A: return AKEYCODE_A;
		case KEY_B: return AKEYCODE_B;
		case KEY_C: return AKEYCODE_C;
		case KEY_D: return AKEYCODE_D;
		case KEY_E: return AKEYCODE_E;
		case KEY_F: return AKEYCODE_F;
		case KEY_G: return AKEYCODE_G;
		case KEY_H: return AKEYCODE_H;
		case KEY_I: return AKEYCODE_I;
		case KEY_J: return AKEYCODE_J;
		case KEY_K: return AKEYCODE_K;
		case KEY_L: return AKEYCODE_L;
		case KEY_M: return AKEYCODE_M;
		case KEY_N: return AKEYCODE_N;
		case KEY_O: return AKEYCODE_O;
		case KEY_P: return AKEYCODE_P;
		case KEY_Q: return AKEYCODE_Q;
		case KEY_R: return AKEYCODE_R;
		case KEY_S: return AKEYCODE_S;
		case KEY_T: return AKEYCODE_T;
		case KEY_U: return AKEYCODE_U;
		case KEY_V: return AKEYCODE_V;
		case KEY_W: return AKEYCODE_W;
		case KEY_X: return AKEYCODE_X;
		case KEY_Y: return AKEYCODE_Y;
		case KEY_Z: return AKEYCODE_Z;
			//Numpad 0
		case  KEY_NUMPAD_0: return AKEYCODE_NUMPAD_0;
		case  KEY_NUMPAD_1: return AKEYCODE_NUMPAD_1;
		case  KEY_NUMPAD_2: return AKEYCODE_NUMPAD_2;
		case  KEY_NUMPAD_3: return AKEYCODE_NUMPAD_3;
		case  KEY_NUMPAD_4: return AKEYCODE_NUMPAD_4;
		case  KEY_NUMPAD_5: return AKEYCODE_NUMPAD_5;
		case  KEY_NUMPAD_6: return AKEYCODE_NUMPAD_6;
		case  KEY_NUMPAD_7: return AKEYCODE_NUMPAD_7;
		case  KEY_NUMPAD_8: return AKEYCODE_NUMPAD_8;
		case  KEY_NUMPAD_9: return AKEYCODE_NUMPAD_9;
			//F1-F12
		case KEY_F1: return AKEYCODE_F1;
		case KEY_F2: return AKEYCODE_F2;
		case KEY_F3: return AKEYCODE_F3;
		case KEY_F4: return AKEYCODE_F4;
		case KEY_F5: return AKEYCODE_F5;
		case KEY_F6: return AKEYCODE_F6;
		case KEY_F7: return AKEYCODE_F7;
		case KEY_F8: return AKEYCODE_F8;
		case KEY_F9: return AKEYCODE_F9 ;
		case KEY_F10: return AKEYCODE_F10;
		case KEY_F11: return AKEYCODE_F11;
		case KEY_F12: return AKEYCODE_F12;
			//Special
		case KEY_LEFT_SHIFT: return AKEYCODE_SHIFT_LEFT;
		case KEY_LEFT_CTRL: return AKEYCODE_CTRL_LEFT;
		case KEY_RIGHT_SHIFT: return AKEYCODE_SHIFT_RIGHT;
		case KEY_RIGHT_CTRL: return KEY_RIGHT_CTRL;
		case KEY_LEFT_ALT: return AKEYCODE_ALT_LEFT;
		case KEY_ESCAPE: return AKEYCODE_ESCAPE;
			//OS-Keys
		case KEY_PAGE_UP: return AKEYCODE_PAGE_UP;
		case KEY_PAGE_DOWN: return AKEYCODE_PAGE_DOWN;
		case KEY_HOME: return AKEYCODE_HOME;
		case KEY_HELP: return AKEYCODE_HELP;
		case KEY_PAUSE: return AKEYCODE_BREAK;
			//Textmodifiers
		case KEY_INSERT: return AKEYCODE_INSERT;
		case KEY_DELETE: return AKEYCODE_DEL;
		case KEY_BACKSPACE: return AKEYCODE_FORWARD_DEL;
		case KEY_TAB: return AKEYCODE_TAB;
		case KEY_CLEAR: return AKEYCODE_CLEAR;
		case KEY_ENTER: return AKEYCODE_ENTER;
		case KEY_SPACE: return AKEYCODE_SPACE;
		case KEY_COMMA: return AKEYCODE_COMMA;
		case KEY_PERIOD: return AKEYCODE_PERIOD;
		case KEY_SLASH: return AKEYCODE_SLASH;
		case KEY_BACKSLASH: return AKEYCODE_BACKSLASH;
		case KEY_SEMI_COLON: return AKEYCODE_SEMICOLON;
			//Math
		case KEY_MINUS: return AKEYCODE_MINUS;
		case KEY_PLUS: return AKEYCODE_PLUS;
		case KEY_MULTIPLY: return AKEYCODE_STAR;
			//Brackets
		case KEY_LEFT_BRACKET: return AKEYCODE_LEFT_BRACKET;
		case KEY_RIGHT_BRACKET: return AKEYCODE_RIGHT_BRACKET;
			//Toggle
		case KEY_NUMLOCK: return AKEYCODE_NUM_LOCK;
		case KEY_CAPSLOCK: return AKEYCODE_CAPS_LOCK;
		case KEY_SCROLLOCK: return AKEYCODE_SCROLL_LOCK;
		default: return KEY_UNKNOWN;
		}
	}
}

#endif