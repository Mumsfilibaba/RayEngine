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

#include "..\..\Include\Utilities\EngineUtilities.h"
#include <sstream>
#include <fstream>

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	std::string ReadFullFile(const std::string& filename, const std::string& filepath)
	{
		std::ifstream file(filepath + filename, std::ios::in);
		if (file.is_open())
		{
			std::stringstream buf;
			buf << file.rdbuf();

			file.close();

			return buf.str();
		}

		return std::string();
	}



	/////////////////////////////////////////////////////////////
	float16 FloatToHalf(float32 f)
	{
		float16 result = 0;
		int32 flt32 = *reinterpret_cast<int32*>(&f);
		
		result = ((flt32 & 0x7fffffff) >> 13) - (0x38000000 >> 13);
		result |= ((flt32 & 0x80000000) >> 16);

		return result;
	}



	/////////////////////////////////////////////////////////////
	float16* FloatToHalf(float32* f, int32 count)
	{
		float16* result = new float16[count];
		for (int32 i = 0; i < count; i++)
		{
			int32 flt32 = *reinterpret_cast<int32*>(f + i);

			result[i] = ((flt32 & 0x7fffffff) >> 13) - (0x38000000 >> 13);
			result[i] |= ((flt32 & 0x80000000) >> 16);
		}

		return result;
	}
}