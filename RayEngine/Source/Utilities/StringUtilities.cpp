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
#include "../../Include/Utilities/StringUtilities.h"

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::wstring WidenString(std::string str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(str);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::string CompressString(std::wstring str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.to_bytes(str);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::string MouseButtonToString(MOUSEBUTTON button)
	{
		switch (button)
		{
		case MOUSEBUTTON_LEFT: return "LEFT";
		case MOUSEBUTTON_MIDDLE: return "MIDDLE";
		case MOUSEBUTTON_RIGHT: return "RIGHT";
		case MOUSEBUTTON_FORWARD: return "FORWARD";
		case MOUSEBUTTON_BACK: return "BACK";
		default: return "UNKNOWN";
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::string KeyCodeToString(KEY keycode)
	{
		switch (keycode)
		{
		//Arrow
		case KEY_LEFT:			return "LEFT";
		case KEY_UP:			return "UP";
		case KEY_RIGHT:			return "RIGHT";
		case KEY_DOWN:			return "DOWN";
		//0-9
		case KEY_0:				return "0";
		case KEY_1:				return "1";
		case KEY_2:				return "2";
		case KEY_3:				return "3";
		case KEY_4:				return "4";
		case KEY_5:				return "5";
		case KEY_6:				return "6";
		case KEY_7:				return "7";
		case KEY_8:				return "8";
		case KEY_9:				return "9";
		//A-Z
		case KEY_A:				return "A";
		case KEY_B:				return "B";
		case KEY_C:				return "C";
		case KEY_D:				return "D";
		case KEY_E:				return "E";
		case KEY_F:				return "F";
		case KEY_G:				return "G";
		case KEY_H:				return "H";
		case KEY_I:				return "I";
		case KEY_J:				return "J";
		case KEY_K:				return "K";
		case KEY_L:				return "L";
		case KEY_M:				return "M";
		case KEY_N:				return "N";
		case KEY_O:				return "O";
		case KEY_P:				return "P";
		case KEY_Q:				return "Q";
		case KEY_R:				return "R";
		case KEY_S:				return "S";
		case KEY_T:				return "T";
		case KEY_U:				return "U";
		case KEY_V:				return "V";
		case KEY_W:				return "W";
		case KEY_X:				return "X";
		case KEY_Y:				return "Y";
		case KEY_Z:				return "Z";
		//Numpad 0
		case KEY_NUMPAD_0:		return "NUMPAD_0";
		case KEY_NUMPAD_1:		return "NUMPAD_1";
		case KEY_NUMPAD_2:		return "NUMPAD_2";
		case KEY_NUMPAD_3:		return "NUMPAD_3";
		case KEY_NUMPAD_4:		return "NUMPAD_4";
		case KEY_NUMPAD_5:		return "NUMPAD_5";
		case KEY_NUMPAD_6:		return "NUMPAD_6";
		case KEY_NUMPAD_7:		return "NUMPAD_7";
		case KEY_NUMPAD_8:		return "NUMPAD_8";
		case KEY_NUMPAD_9:		return "NUMPAD_9";
		//F1-F24
		case KEY_F1:			return "F1";
		case KEY_F2:			return "F2";
		case KEY_F3:			return "F3";
		case KEY_F4:			return "F4";
		case KEY_F5:			return "F5";
		case KEY_F6:			return "F6";
		case KEY_F7:			return "F7";
		case KEY_F8:			return "F8";
		case KEY_F9:			return "F9";
		case KEY_F10:			return "F10";
		case KEY_F11:			return "F11";
		case KEY_F12:			return "F12";
		case KEY_F13:			return "F13";
		case KEY_F14:			return "F14";
		case KEY_F15:			return "F15";
		case KEY_F16:			return "F16";
		case KEY_F17:			return "F17";
		case KEY_F18:			return "F18";
		case KEY_F19:			return "F19";
		case KEY_F20:			return "F20";
		case KEY_F21:			return "F21";
		case KEY_F22:			return "F22";
		case KEY_F23:			return "F23";
		case KEY_F24:			return "F24";
		//Special
		case KEY_CTRL:			return "CTRL";
		case KEY_SHIFT:			return "SHIFT";
		case KEY_LEFT_SHIFT:	return "LEFT_SHIFT";
		case KEY_LEFT_CTRL:		return "LEFT_CTRL";
		case KEY_RIGHT_SHIFT:	return "RIGHT_SHIFT";
		case KEY_RIGHT_CTRL:	return "RIGHT_CTRL";
		case KEY_ALT:			return "ALT";
		case KEY_LEFT_ALT:		return "LEFT_ALT";
		case KEY_ALTGR:			return "ALTGR";
		case KEY_ESCAPE:		return "ESCAPE";
		//OS-Keys
		case KEY_PAGE_UP:		return "PAGE_UP";
		case KEY_PAGE_DOWN:		return "PAGE_DOWN";
		case KEY_END:			return "END";
		case KEY_HOME:			return "HOME";
		case KEY_LEFT_CMD:		return "LEFT_CMD";
		case KEY_RIGHT_CMD:		return "RIGHT_CMD";
		case KEY_APPS:			return "APPS";
		case KEY_HELP:			return "HELP";
		case KEY_PAUSE:			return "PAUSE";
		//Textmodifiers
		case KEY_INSERT:		return "INSERT";
		case KEY_DELETE:		return "DELETE";
		case KEY_BACKSPACE:		return "BACKSPACE";
		case KEY_TAB:			return "TAB";
		case KEY_CLEAR:			return "CLEAR";
		case KEY_ENTER:			return "ENTER";
		case KEY_SPACE:			return "SPACE";
		case KEY_COMMA:			return "COMMA";
		case KEY_PERIOD:		return "PERIOD";
		case KEY_TILDE:			return "TILDE";
		case KEY_QUOTE:			return "QUOTE";
		case KEY_SLASH:			return "SLASH";
		case KEY_BACKSLASH:		return "BACKSLASH";
		case KEY_SEMI_COLON:	return "SEMI_COLON";
		case KEY_DECIMAL:		return "DECIMAL";
		//Math
		case KEY_MINUS:			return "MINUS";
		case KEY_PLUS:			return "PLUS";
		case KEY_DIVIDE:		return "DIVIDE";
		case KEY_MULTIPLY:		return "MULTIPLY";
		case KEY_SUBTRACT:		return "SUBTRACT";
		case KEY_ADD:			return "ADD";
		//Brackets
		case KEY_LEFT_BRACKET:	return "LEFT_BRACKET";
		case KEY_RIGHT_BRACKET: return "RIGHT_BRACKET";
		case KEY_ANGLE_BRACKET: return "ANGLE_BRACKET";
		//Toggle
		case KEY_NUMLOCK:		return "NUMLOCK";
		case KEY_CAPSLOCK:		return "CAPSLOCK";
		case KEY_SCROLLOCK:		return "SCROLLOCK";
		default: return "UNKNOWN";
		}
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::string FormatToString(FORMAT format)
	{
		switch (format)
		{
		case FORMAT_UNKNOWN: return "FORMAT_UNKNOWN";
		case FORMAT_R32G32B32A32_FLOAT: return "FORMAT_R32G32B32A32_FLOAT";
		case FORMAT_R32G32B32A32_UINT: return "FORMAT_R32G32B32A32_UINT";
		case FORMAT_R32G32B32A32_SINT: return "FORMAT_R32G32B32A32_SINT";
		case FORMAT_R32G32B32_FLOAT: return "FORMAT_R32G32B32_FLOAT";
		case FORMAT_R32G32B32_UINT: return "FORMAT_R32G32B32_UINT";
		case FORMAT_R32G32B32_SINT: return "FORMAT_R32G32B32_SINT";
		case FORMAT_R16G16B16A16_FLOAT: return "FORMAT_R16G16B16A16_FLOAT";
		case FORMAT_R16G16B16A16_UNORM: return "FORMAT_R16G16B16A16_UNORM";
		case FORMAT_R16G16B16A16_UINT: return "FORMAT_R16G16B16A16_UINT";
		case FORMAT_R16G16B16A16_SNORM: return "FORMAT_R16G16B16A16_SNORM";
		case FORMAT_R16G16B16A16_SINT: return "FORMAT_R16G16B16A16_SINT";
		case FORMAT_R32G32_FLOAT: return "FORMAT_R32G32_FLOAT";
		case FORMAT_R32G32_UINT: return "FORMAT_R32G32_UINT";
		case FORMAT_R32G32_SINT: return "FORMAT_R32G32_SINT";
		case FORMAT_D32_FLOAT_S8X24_UINT: return "FORMAT_D32_FLOAT_S8X24_UINT";
		case FORMAT_R8G8B8A8_TYPELESS: return "FORMAT_R8G8B8A8_TYPELESS";
		case FORMAT_R8G8B8A8_UNORM: return "FORMAT_R8G8B8A8_UNORM";
		case FORMAT_R8G8B8A8_UNORM_SRGB: return "FORMAT_R8G8B8A8_UNORM_SRGB";
		case FORMAT_R8G8B8A8_UINT: return "FORMAT_R8G8B8A8_UINT";
		case FORMAT_R8G8B8A8_SNORM: return "FORMAT_R8G8B8A8_SNORM";
		case FORMAT_R8G8B8A8_SINT: return "FORMAT_R8G8B8A8_SINT";
		case FORMAT_R16G16_FLOAT: return "FORMAT_R16G16_FLOAT";
		case FORMAT_R16G16_UNORM: return "FORMAT_R16G16_UNORM";
		case FORMAT_R16G16_UINT: return "FORMAT_R16G16_UINT";
		case FORMAT_R16G16_SNORM: return "FORMAT_R16G16_SNORM";
		case FORMAT_R16G16_SINT: return "FORMAT_R16G16_SINT";
		case FORMAT_D32_FLOAT: return "FORMAT_D32_FLOAT";
		case FORMAT_R32_FLOAT: return "FORMAT_R32_FLOAT";
		case FORMAT_R32_UINT: return "FORMAT_R32_UINT";
		case FORMAT_R32_SINT: return "FORMAT_R32_SINT";
		case FORMAT_D24_UNORM_S8_UINT: return "FORMAT_D24_UNORM_S8_UINT";
		case FORMAT_R8G8_UNORM: return "FORMAT_R8G8_UNORM";
		case FORMAT_R8G8_UINT: return "FORMAT_R8G8_UINT";
		case FORMAT_R8G8_SNORM: return "FORMAT_R8G8_SNORM";
		case FORMAT_R8G8_SINT: return "FORMAT_R8G8_SINT";
		case FORMAT_R16_FLOAT: return "FORMAT_R16_FLOAT";
		case FORMAT_D16_UNORM: return "FORMAT_D16_UNORM";
		case FORMAT_R16_UNORM: return "FORMAT_R16_UNORM";
		case FORMAT_R16_UINT: return "FORMAT_R16_UINT";
		case FORMAT_R16_SNORM: return "FORMAT_R16_SNORM";
		case FORMAT_R16_SINT: return "FORMAT_R16_SINT";
		case FORMAT_R8_UNORM: return "FORMAT_R8_UNORM";
		case FORMAT_R8_UINT: return "FORMAT_R8_UINT";
		case FORMAT_R8_SNORM: return "FORMAT_R8_SNORM";
		case FORMAT_R8_SINT: return "FORMAT_R8_SINT";
		case FORMAT_B8G8R8A8_UNORM: return "FORMAT_B8G8R8A8_UNORM";
		case FORMAT_B8G8R8A8_UNORM_SRGB: return "FORMAT_B8G8R8A8_UNORM_SRGB";
		default: return "FORMAT_UNKNOWN";
		}
	}
}