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
#include "..\Defines.h"
#include "..\Types.h"

namespace RayEngine
{
	namespace Graphics
	{
		struct Viewport
		{
		public:
			float Width = 0.0f;
			float Height = 0.0f;
			float TopLeftX = 0.0f;
			float TopLeftY = 0.0f;
			float MinDepth = 0.0f;
			float MaxDepth = 1.0f;

		public:
			inline bool operator==(const Viewport& other)
			{
				return (Width == other.Width) && (Height == other.Height) && (TopLeftX == other.TopLeftX) && 
					(TopLeftY == other.TopLeftY) && (MinDepth == other.MinDepth) && (MaxDepth == other.MaxDepth);
			}

			inline bool operator!=(const Viewport& other)
			{
				return !(*this == other);
			}
		};
	}
}