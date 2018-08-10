#include "..\..\Include\Utilities\StringUtilities.h"
#include <codecvt>

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	std::wstring WidenString(std::string str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(str);
	}



	/////////////////////////////////////////////////////////////
	std::string CompressString(std::wstring str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.to_bytes(str);
	}
}