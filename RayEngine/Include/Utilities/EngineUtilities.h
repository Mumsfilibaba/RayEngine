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

#pragma once
#include "../Defines.h"
#include "../Types.h"
#include <string>

namespace RayEngine
{
	/*////////////////////////////////////////////////////////////

		Reads a full file and returns it as a string. 
		
		filename - Name of the file with extension.

		filepath - Path of the file. Results in filepath + filename.

	////////////////////////////////////////////////////////////*/
	std::string ReadFullFile(const std::string& filename, const std::string& filepath);

	/*////////////////////////////////////////////////////////////

		Converts a float to half (float32 to float16).

		f - float32 valaue to be converted.

	////////////////////////////////////////////////////////////*/
	float16 FloatToHalf(float32 f);
	
	/*////////////////////////////////////////////////////////////

		Converts an array of floats to halfs (float32 to float16)
		Caller of the function is responsible for calling 
		delete[] when finished with the memory.

		pF - float32 array to be converted.

		count - Number of elements in the array.

	////////////////////////////////////////////////////////////*/
	float16* FloatToHalf(float32* pF, int32 count);
}