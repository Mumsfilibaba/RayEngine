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
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct Viewport
		{
		public:
			float Width;
			float Height;
			float TopLeftX;
			float TopLeftY;
			float MinDepth;
			float MaxDepth;

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


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct Rect
		{
		public:
			int32 Left;
			int32 Top;
			int32 Right;
			int32 Bottom;

		public:
			inline bool operator==(const Rect& other)
			{
				return (Left == other.Left) && (Top == other.Top) && (Right == other.Right) && (Bottom == other.Bottom);
			}

			inline bool operator!=(const Rect& other)
			{
				return !(*this == other);
			}
		};
	}
}