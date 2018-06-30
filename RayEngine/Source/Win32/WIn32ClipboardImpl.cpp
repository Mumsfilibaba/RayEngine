#include "..\..\Include\System\Clipboard.h"

#if defined(RE_PLATFORM_WINDOWS)

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN 1
#endif

#include <Windows.h>

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
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



	/////////////////////////////////////////////////////////////
	void Clipboard::SetString(const std::string& string)
	{
		const Tchar* text = string.data();
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