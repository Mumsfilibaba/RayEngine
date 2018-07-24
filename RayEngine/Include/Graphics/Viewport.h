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