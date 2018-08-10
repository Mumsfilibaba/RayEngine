#pragma once

#include <string>

namespace RayEngine
{
	std::wstring WidenString(std::string str);
	std::string	 CompressString(std::wstring);
}