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
	class Clipboard
	{
		RE_STATIC_CLASS(Clipboard);

	public:
	    /*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            Returns the current string in the clipboard
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		static std::string GetString();
		
		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            
			Sets the current string in the clipboard

			string - string that will be the new contents of the clipboard

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		static void SetString(const std::string& string);
	};
}