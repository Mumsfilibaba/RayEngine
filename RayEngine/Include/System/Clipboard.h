#pragma once

#include "..\Defines.h"
#include "..\Types.h"
#include <string>

namespace RayEngine
{
	class Clipboard
	{
	public:

	public:
		static std::string GetString();
		static void SetString(const std::string& string);
	};
}