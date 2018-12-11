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
#include "Win32KeyCodes.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	KEY Win32ToRe(int32 keyCode)
	{
		switch (keyCode)
		{
		//Arrow
		case 0x25: return KEY_LEFT;
		case 0x26: return KEY_UP;
		case 0x27: return KEY_RIGHT;
		case 0x28: return KEY_DOWN;
		//0-9
		case 0x30: return KEY_0;
		case 0x31: return KEY_1;
		case 0x32: return KEY_2;
		case 0x33: return KEY_3;
		case 0x34: return KEY_4;
		case 0x35: return KEY_5;
		case 0x36: return KEY_6;
		case 0x37: return KEY_7;
		case 0x38: return KEY_8;
		case 0x39: return KEY_9;
		//A-Z
		case 0x41: return KEY_A;
		case 0x42: return KEY_B;
		case 0x43: return KEY_C;
		case 0x44: return KEY_D;
		case 0x45: return KEY_E;
		case 0x46: return KEY_F;
		case 0x47: return KEY_G;
		case 0x48: return KEY_H;
		case 0x49: return KEY_I;
		case 0x4A: return KEY_J;
		case 0x4B: return KEY_K;
		case 0x4C: return KEY_L;
		case 0x4D: return KEY_M;
		case 0x4E: return KEY_N;
		case 0x4F: return KEY_O;
		case 0x50: return KEY_P;
		case 0x51: return KEY_Q;
		case 0x52: return KEY_R;
		case 0x53: return KEY_S;
		case 0x54: return KEY_T;
		case 0x55: return KEY_U;
		case 0x56: return KEY_V;
		case 0x57: return KEY_W;
		case 0x58: return KEY_X;
		case 0x59: return KEY_Y;
		case 0x5A: return KEY_Z;
		//Numpad 0
		case 0x60: return KEY_NUMPAD_0;
		case 0x61: return KEY_NUMPAD_1;
		case 0x62: return KEY_NUMPAD_2;
		case 0x63: return KEY_NUMPAD_3;
		case 0x64: return KEY_NUMPAD_4;
		case 0x65: return KEY_NUMPAD_5;
		case 0x66: return KEY_NUMPAD_6;
		case 0x67: return KEY_NUMPAD_7;
		case 0x68: return KEY_NUMPAD_8;
		case 0x69: return KEY_NUMPAD_9;
		//F1-F24
		case 0x70: return KEY_F1;
		case 0x71: return KEY_F2;
		case 0x72: return KEY_F3;
		case 0x73: return KEY_F4;
		case 0x74: return KEY_F5;
		case 0x75: return KEY_F6;
		case 0x76: return KEY_F7;
		case 0x77: return KEY_F8;
		case 0x78: return KEY_F9;
		case 0x79: return KEY_F10;
		case 0x7A: return KEY_F11;
		case 0x7B: return KEY_F12;
		case 0x7C: return KEY_F13;
		case 0x7D: return KEY_F14;
		case 0x7E: return KEY_F15;
		case 0x7F: return KEY_F16;
		case 0x80: return KEY_F17;
		case 0x81: return KEY_F18;
		case 0x82: return KEY_F19;
		case 0x83: return KEY_F20;
		case 0x84: return KEY_F21;
		case 0x85: return KEY_F22;
		case 0x86: return KEY_F23;
		case 0x87: return KEY_F24;
		//Special
		case 0x11: return KEY_CTRL;
		case 0x10: return KEY_SHIFT;
		case 0xA0: return KEY_LEFT_SHIFT;
		case 0xA2: return KEY_LEFT_CTRL;
		case 0xA1: return KEY_RIGHT_SHIFT;
		case 0xA3: return KEY_RIGHT_CTRL;
		case 0x12: return KEY_ALT;
		case 0xA4: return KEY_LEFT_ALT;
		case 0xA5: return KEY_ALTGR;
		case 0x1B: return KEY_ESCAPE;
		//OS-Keys
		case 0x21: return KEY_PAGE_UP;
		case 0x22: return KEY_PAGE_DOWN;
		case 0x23: return KEY_END;
		case 0x24: return KEY_HOME;
		case 0x5B: return KEY_LEFT_CMD;
		case 0x5C: return KEY_RIGHT_CMD;
		case 0x5D: return KEY_APPS;
		case 0x2F: return KEY_HELP;
		case 0x13: return KEY_PAUSE;
		//Textmodifiers
		case 0x2D: return KEY_INSERT;
		case 0x2E: return KEY_DELETE;
		case 0x08: return KEY_BACKSPACE;
		case 0x09: return KEY_TAB;
		case 0x0C: return KEY_CLEAR;
		case 0x0D: return KEY_ENTER;
		case 0x20: return KEY_SPACE;
		case 0xBC: return KEY_COMMA;
		case 0xBE: return KEY_PERIOD;
		case 0xC0: return KEY_TILDE;
		case 0xDE: return KEY_QUOTE;
		case 0xBF: return KEY_SLASH;
		case 0xDC: return KEY_BACKSLASH;
		case 0xBA: return KEY_SEMI_COLON;
		case 0x6E: return KEY_DECIMAL;
		//Math
		case 0xBD: return KEY_MINUS;
		case 0xBB: return KEY_PLUS;
		case 0x6F: return KEY_DIVIDE;
		case 0x6A: return KEY_MULTIPLY;
		case 0x6D: return KEY_SUBTRACT;
		case 0x6B: return KEY_ADD;
		//Brackets
		case 0xDD: return KEY_LEFT_BRACKET;
		case 0xDB: return KEY_RIGHT_BRACKET;
		case 0xE2: return KEY_ANGLE_BRACKET;
		//Toggle
		case 0x90: return KEY_NUMLOCK;
		case 0x14: return KEY_CAPSLOCK;
		case 0x91: return KEY_SCROLLOCK;
		default: return KEY_UNKNOWN;
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int32 ReToWin32(KEY keyCode)
	{
		switch (keyCode)
		{
		//Arrow
		case KEY_LEFT:			return 0x25;
		case KEY_UP:			return 0x26;
		case KEY_RIGHT:			return 0x27;
		case KEY_DOWN:			return 0x28;
		//0-9
		case KEY_0:				return 0x30;
		case KEY_1:				return 0x31;
		case KEY_2:				return 0x32;
		case KEY_3:				return 0x33;
		case KEY_4:				return 0x34;
		case KEY_5:				return 0x35;
		case KEY_6:				return 0x36;
		case KEY_7:				return 0x37;
		case KEY_8:				return 0x38;
		case KEY_9:				return 0x39;
		//A-Z
		case KEY_A:				return 0x41;
		case KEY_B:				return 0x42;
		case KEY_C:				return 0x43;
		case KEY_D:				return 0x44;
		case KEY_E:				return 0x45;
		case KEY_F:				return 0x46;
		case KEY_G:				return 0x47;
		case KEY_H:				return 0x48;
		case KEY_I:				return 0x49;
		case KEY_J:				return 0x4A;
		case KEY_K:				return 0x4B;
		case KEY_L:				return 0x4C;
		case KEY_M:				return 0x4D;
		case KEY_N:				return 0x4E;
		case KEY_O:				return 0x4F;
		case KEY_P:				return 0x50;
		case KEY_Q:				return 0x51;
		case KEY_R:				return 0x52;
		case KEY_S:				return 0x53;
		case KEY_T:				return 0x54;
		case KEY_U:				return 0x55;
		case KEY_V:				return 0x56;
		case KEY_W:				return 0x57;
		case KEY_X:				return 0x58;
		case KEY_Y:				return 0x59;
		case KEY_Z:				return 0x5A;
		//Numpad 0
		case KEY_NUMPAD_0:		return 0x60;
		case KEY_NUMPAD_1:		return 0x61;
		case KEY_NUMPAD_2:		return 0x62;
		case KEY_NUMPAD_3:		return 0x63;
		case KEY_NUMPAD_4:		return 0x64;
		case KEY_NUMPAD_5:		return 0x65;
		case KEY_NUMPAD_6:		return 0x66;
		case KEY_NUMPAD_7:		return 0x67;
		case KEY_NUMPAD_8:		return 0x68;
		case KEY_NUMPAD_9:		return 0x69;
		//F1-F24
		case KEY_F1:			return 0x70;
		case KEY_F2:			return 0x71;
		case KEY_F3:			return 0x72;
		case KEY_F4:			return 0x73;
		case KEY_F5:			return 0x74;
		case KEY_F6:			return 0x75;
		case KEY_F7:			return 0x76;
		case KEY_F8:			return 0x77;
		case KEY_F9:			return 0x78;
		case KEY_F10:			return 0x79;
		case KEY_F11:			return 0x7A;
		case KEY_F12:			return 0x7B;
		case KEY_F13:			return 0x7C;
		case KEY_F14:			return 0x7D;
		case KEY_F15:			return 0x7E;
		case KEY_F16:			return 0x7F;
		case KEY_F17:			return 0x80;
		case KEY_F18:			return 0x81;
		case KEY_F19:			return 0x82;
		case KEY_F20:			return 0x83;
		case KEY_F21:			return 0x84;
		case KEY_F22:			return 0x85;
		case KEY_F23:			return 0x86;
		case KEY_F24:			return 0x87;
		//Special
		case KEY_CTRL:			return 0x11;
		case KEY_SHIFT:			return 0x10;
		case KEY_LEFT_SHIFT:	return 0xA0;
		case KEY_LEFT_CTRL:		return 0xA2;
		case KEY_RIGHT_SHIFT:	return 0xA1;
		case KEY_RIGHT_CTRL:	return 0xA3;
		case KEY_ALT:			return 0x12;
		case KEY_LEFT_ALT:		return 0xA4;
		case KEY_ALTGR:			return 0xA5;
		case KEY_ESCAPE:		return 0x1B;
		//OS-Keys
		case KEY_PAGE_UP:		return 0x21;
		case KEY_PAGE_DOWN:		return 0x22;
		case KEY_END:			return 0x23;
		case KEY_HOME:			return 0x24;
		case KEY_LEFT_CMD:		return 0x5B;
		case KEY_RIGHT_CMD:		return 0x5C;
		case KEY_APPS:			return 0x5D;
		case KEY_HELP:			return 0x2F;
		case KEY_PAUSE:			return 0x13;
		//Textmodifiers
		case KEY_INSERT:		return 0x2D;
		case KEY_DELETE:		return 0x2E;
		case KEY_BACKSPACE:		return 0x08;
		case KEY_TAB:			return 0x09;
		case KEY_CLEAR:			return 0x0C;
		case KEY_ENTER:			return 0x0D;
		case KEY_SPACE:			return 0x20;
		case KEY_COMMA:			return 0xBC;
		case KEY_PERIOD:		return 0xBE;
		case KEY_TILDE:			return 0xC0;
		case KEY_QUOTE:			return 0xDE;
		case KEY_SLASH:			return 0xBF;
		case KEY_BACKSLASH:		return 0xDC;
		case KEY_SEMI_COLON:	return 0xBA;
		case KEY_DECIMAL:		return 0x6E;
		//Math
		case KEY_MINUS:			return 0xBD;
		case KEY_PLUS:			return 0xBB;
		case KEY_DIVIDE:		return 0x6F;
		case KEY_MULTIPLY:		return 0x6A;
		case KEY_SUBTRACT:		return 0x6D;
		case KEY_ADD:			return 0x6B;
		//Brackets
		case KEY_LEFT_BRACKET:	return 0xDD;
		case KEY_RIGHT_BRACKET: return 0xDB;
		case KEY_ANGLE_BRACKET: return 0xE2;
		//Toggle
		case KEY_NUMLOCK:		return 0x90;
		case KEY_CAPSLOCK:		return 0x14;
		case KEY_SCROLLOCK:		return 0x91;
		default: return 0;
		}
	}
}

#endif