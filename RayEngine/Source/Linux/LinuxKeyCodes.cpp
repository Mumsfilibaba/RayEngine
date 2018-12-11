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
#include "LinuxKeyCodes.h"

#if defined(RE_PLATFORM_LINUX)

#include <iostream>

namespace RayEngine
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    KEY XKeyEventToKeycode(XKeyEvent* pXEvent)
    {
        KEY keycode = KEY_UNKNOWN;
        for (int32 i = 0; i < 4; i++)
        {
            keycode = XToKeycode(XLookupKeysym(pXEvent, i));
            if (keycode != KEY_UNKNOWN)
                break;
        }

        return keycode;
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    KEY XToKeycode(KeySym keySym)
    {
        switch(keySym)
        {
        //Arrow
		case XK_Left:           return KEY_LEFT;
		case XK_Up:             return KEY_UP;
		case XK_Right:          return KEY_RIGHT;
		case XK_Down:           return KEY_DOWN;
		//0-9
		case XK_0:              return KEY_0;
		case XK_1:              return KEY_1;
		case XK_2:              return KEY_2;
		case XK_3:              return KEY_3;
		case XK_4:              return KEY_4;
		case XK_5:              return KEY_5;
		case XK_6:              return KEY_6;
		case XK_7:              return KEY_7;
		case XK_8:              return KEY_8;
		case XK_9:              return KEY_9;
		//A-Z
		case XK_a:              return KEY_A;
		case XK_b:              return KEY_B;
		case XK_c:              return KEY_C;
		case XK_d:              return KEY_D;
		case XK_e:              return KEY_E;
		case XK_f:              return KEY_F;
		case XK_g:              return KEY_G;
		case XK_h:              return KEY_H;
		case XK_i:              return KEY_I;
		case XK_j:              return KEY_J;
		case XK_k:              return KEY_K;
		case XK_l:              return KEY_L;
		case XK_m:              return KEY_M;
		case XK_n:              return KEY_N;
		case XK_o:              return KEY_O;
		case XK_p:              return KEY_P;
		case XK_q:              return KEY_Q;
		case XK_r:              return KEY_R;
		case XK_s:              return KEY_S;
		case XK_t:              return KEY_T;
		case XK_u:              return KEY_U;
		case XK_v:              return KEY_V;
		case XK_w:              return KEY_W;
		case XK_x:              return KEY_X;
		case XK_y:              return KEY_Y;
		case XK_z:              return KEY_Z;
		//Numpad 0
		case XK_KP_Insert:      return KEY_NUMPAD_0;
		case XK_KP_End:         return KEY_NUMPAD_1;
		case XK_KP_Down:        return KEY_NUMPAD_2;
		case XK_KP_Page_Down:   return KEY_NUMPAD_3;
		case XK_KP_Left:        return KEY_NUMPAD_4;
		case XK_KP_Begin:       return KEY_NUMPAD_5;
		case XK_KP_Right:       return KEY_NUMPAD_6;
		case XK_KP_Home:        return KEY_NUMPAD_7;
		case XK_KP_Up:          return KEY_NUMPAD_8;
		case XK_KP_Page_Up:     return KEY_NUMPAD_9;
		//F1-F24
		case XK_F1:             return KEY_F1;
		case XK_F2:             return KEY_F2;
		case XK_F3:             return KEY_F3;
		case XK_F4:             return KEY_F4;
		case XK_F5:             return KEY_F5;
		case XK_F6:             return KEY_F6;
		case XK_F7:             return KEY_F7;
		case XK_F8:             return KEY_F8;
		case XK_F9:             return KEY_F9;
		case XK_F10:            return KEY_F10;
		case XK_F11:            return KEY_F11;
		case XK_F12:            return KEY_F12;
		case XK_F13:            return KEY_F13;
		case XK_F14:            return KEY_F14;
		case XK_F15:            return KEY_F15;
		case XK_F16:            return KEY_F16;
		case XK_F17:            return KEY_F17;
		case XK_F18:            return KEY_F18;
		case XK_F19:            return KEY_F19;
		case XK_F20:            return KEY_F20;
		case XK_F21:            return KEY_F21;
		case XK_F22:            return KEY_F22;
		case XK_F23:            return KEY_F23;
		case XK_F24:            return KEY_F24;
		//Special
		case XK_Shift_L:        return KEY_LEFT_SHIFT;
		case XK_Control_L:      return KEY_LEFT_CTRL;
		case XK_Shift_R:        return KEY_RIGHT_SHIFT;
		case XK_Control_R:      return KEY_RIGHT_CTRL;
		case XK_Alt_R:          return KEY_ALT;
		case XK_Alt_L:          return KEY_LEFT_ALT;
		case XK_Escape:         return KEY_ESCAPE;
		//OS-Keys
		case XK_Prior:          return KEY_PAGE_UP;
		case XK_Next:           return KEY_PAGE_DOWN;
		case XK_End:            return KEY_END;
		case XK_Home:           return KEY_HOME;
		case XK_Super_L:        return KEY_LEFT_CMD;
		case XK_Super_R:        return KEY_RIGHT_CMD;
		case XK_Help:           return KEY_HELP;
		case XK_Pause:          return KEY_PAUSE;
		//Textmodifiers
		case XK_Insert:         return KEY_INSERT;
		case XK_Delete:         return KEY_DELETE;
		case XK_BackSpace:      return KEY_BACKSPACE;
		case XK_Tab:            return KEY_TAB;
		case XK_Clear:          return KEY_CLEAR;
		case XK_Return:         return KEY_ENTER;
		case XK_space:          return KEY_SPACE;
		case XK_comma:          return KEY_COMMA;
		case XK_period:         return KEY_PERIOD;
		case XK_grave:          return KEY_TILDE;
		case XK_apostrophe:     return KEY_QUOTE;
		case XK_slash:          return KEY_SLASH;
		case XK_backslash:      return KEY_BACKSLASH;
		case XK_semicolon:      return KEY_SEMI_COLON;
		case XK_KP_Decimal:     return KEY_DECIMAL;
		//Math
		case XK_minus:          return KEY_MINUS;
		case XK_plus:           return KEY_PLUS;
		case XK_KP_Divide:      return KEY_DIVIDE;   
		case XK_KP_Multiply:    return KEY_MULTIPLY;
		case XK_KP_Subtract:    return KEY_SUBTRACT;
		case XK_KP_Add:         return KEY_ADD;
		//Brackets
		case XK_bracketleft:    return KEY_LEFT_BRACKET;
		case XK_braceright:     return KEY_RIGHT_BRACKET;
		//Toggle
		case XK_Num_Lock:       return KEY_NUMLOCK;
		case XK_Caps_Lock:      return KEY_CAPSLOCK;
		case XK_Scroll_Lock:    return KEY_SCROLLOCK;
		default:
            std::cout << "Value: 0x" << std::hex << keySym << "\n";  
            return KEY_UNKNOWN;
        }
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    KeySym KeycodeToX(KEY keycode)
    {
        switch(keycode)
        {
        //Arrow
		case KEY_LEFT:           return XK_Left;
		case KEY_UP:             return XK_Up;
		case KEY_RIGHT:          return XK_Right;
		case KEY_DOWN:           return XK_Down;
		//0-9
		case KEY_0:              return XK_0;
		case KEY_1:              return XK_1;
		case KEY_2:              return XK_2;
		case KEY_3:              return XK_3;
		case KEY_4:              return XK_4;
		case KEY_5:              return XK_5;
		case KEY_6:              return XK_6;
		case KEY_7:              return XK_7;
		case KEY_8:              return XK_8;
		case KEY_9:              return XK_9;
		//A-Z
		case KEY_A:              return XK_a;
		case KEY_B:              return XK_b;
		case KEY_C:              return XK_c;
		case KEY_D:              return XK_d;
		case KEY_E:              return XK_e;
		case KEY_F:              return XK_f;
		case KEY_G:              return XK_g;
		case KEY_H:              return XK_h;
		case KEY_I:              return XK_i;
		case KEY_J:              return XK_j;
		case KEY_K:              return XK_k;
		case KEY_L:              return XK_l;
		case KEY_M:              return XK_m;
		case KEY_N:              return XK_n;
		case KEY_O:              return XK_o;
		case KEY_P:              return XK_p;
		case KEY_Q:              return XK_q;
		case KEY_R:              return XK_r;
		case KEY_S:              return XK_s;
		case KEY_T:              return XK_t;
		case KEY_U:              return XK_u;
		case KEY_V:              return XK_v;
		case KEY_W:              return XK_w;
		case KEY_X:              return XK_x;
		case KEY_Y:              return XK_y;
		case KEY_Z:              return XK_z;
		//Numpad 0
		case KEY_NUMPAD_0:       return XK_KP_Insert;
		case KEY_NUMPAD_1:       return XK_KP_End;
		case KEY_NUMPAD_2:       return XK_KP_Down;
		case KEY_NUMPAD_3:       return XK_KP_Page_Down;
		case KEY_NUMPAD_4:       return XK_KP_Left;
		case KEY_NUMPAD_5:       return XK_KP_Begin;
		case KEY_NUMPAD_6:       return XK_KP_Right;
		case KEY_NUMPAD_7:       return XK_KP_Home;
		case KEY_NUMPAD_8:       return XK_KP_Up;
		case KEY_NUMPAD_9:       return XK_KP_Page_Up;
		//F1-F24
		case KEY_F1:             return XK_F1;
		case KEY_F2:             return XK_F2;
		case KEY_F3:             return XK_F3;
		case KEY_F4:             return XK_F4;
		case KEY_F5:             return XK_F5;
		case KEY_F6:             return XK_F6;
		case KEY_F7:             return XK_F7;
		case KEY_F8:             return XK_F8;
		case KEY_F9:             return XK_F9;
		case KEY_F10:            return XK_F10;
		case KEY_F11:            return XK_F11;
		case KEY_F12:            return XK_F12;
		case KEY_F13:            return XK_F13;
		case KEY_F14:            return XK_F14;
		case KEY_F15:            return XK_F15;
		case KEY_F16:            return XK_F16;
		case KEY_F17:            return XK_F17;
		case KEY_F18:            return XK_F18;
		case KEY_F19:            return XK_F19;
		case KEY_F20:            return XK_F20;
		case KEY_F21:            return XK_F21;
		case KEY_F22:            return XK_F22;
		case KEY_F23:            return XK_F23;
		case KEY_F24:            return XK_F24;
		//Special
		case KEY_LEFT_SHIFT:     return XK_Shift_L;
		case KEY_LEFT_CTRL:      return XK_Control_L;
		case KEY_RIGHT_SHIFT:    return XK_Shift_R;
		case KEY_RIGHT_CTRL:     return XK_Control_R;
		case KEY_ALT:            return XK_Alt_R;
		case KEY_LEFT_ALT:       return XK_Alt_L;
		case KEY_ESCAPE:         return XK_Escape;
		//OS-Keys
		case KEY_PAGE_UP:        return XK_Prior;
		case KEY_PAGE_DOWN:      return XK_Next;
		case KEY_END:            return XK_End;
		case KEY_HOME:           return XK_Home;
		case KEY_LEFT_CMD:       return XK_Super_L;
		case KEY_RIGHT_CMD:      return XK_Super_R;
		case KEY_HELP:           return XK_Help;
		case KEY_PAUSE:          return XK_Pause;
		//Textmodifiers
		case KEY_INSERT:         return XK_Insert;
		case KEY_DELETE:         return XK_Delete;
		case KEY_BACKSPACE:      return XK_BackSpace;
		case KEY_TAB:            return XK_Tab;
		case KEY_CLEAR:          return XK_Clear;
		case KEY_ENTER:          return XK_Return;
		case KEY_SPACE:          return XK_space;
		case KEY_COMMA:          return XK_comma;
		case KEY_PERIOD:         return XK_period;
		case KEY_TILDE:          return XK_grave;
		case KEY_QUOTE:          return XK_apostrophe;
		case KEY_SLASH:          return XK_slash;
		case KEY_BACKSLASH:      return XK_backslash;
		case KEY_SEMI_COLON:     return XK_semicolon;
		case KEY_DECIMAL:        return XK_KP_Decimal;
		//Math
		case KEY_MINUS:          return XK_minus;
		case KEY_PLUS:           return XK_plus;
		case KEY_DIVIDE:         return XK_KP_Divide;   
		case KEY_MULTIPLY:       return XK_KP_Multiply;
		case KEY_SUBTRACT:       return XK_KP_Subtract;
		case KEY_ADD:            return XK_KP_Add;
		//Brackets
		case KEY_LEFT_BRACKET:   return XK_bracketleft;
		case KEY_RIGHT_BRACKET:  return XK_braceright;
		//Toggle
		case KEY_NUMLOCK:        return XK_Num_Lock;
		case KEY_CAPSLOCK:       return XK_Caps_Lock;
		case KEY_SCROLLOCK:      return XK_Scroll_Lock;
		default:
            return 0;
        }
    }
}

#endif