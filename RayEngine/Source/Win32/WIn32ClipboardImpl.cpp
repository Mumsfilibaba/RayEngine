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

#include "..\..\Include\System\Clipboard.h"

#if defined(RE_PLATFORM_WINDOWS)

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN 1
#endif

#include <Windows.h>

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::string Clipboard::GetString()
	{
		if (OpenClipboard(0) != 0)
		{
			HANDLE hData = GetClipboardData(CF_TEXT);

			const char* text = static_cast<const char*>(GlobalLock(hData));
			
			GlobalUnlock(hData);
			CloseClipboard();

			return std::string(text);
		}

		return std::string();
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Clipboard::SetString(const std::string& string)
	{
		const char* text = string.c_str();
		int32 size = static_cast<int32>(string.size()) + 1;

		HGLOBAL cb = GlobalAlloc(GMEM_MOVEABLE, size);
		memcpy(GlobalLock(cb), text, size);
		GlobalUnlock(cb);

		OpenClipboard(0);
		
		EmptyClipboard();
		SetClipboardData(CF_TEXT, cb);

		CloseClipboard();
	}
}

#endif