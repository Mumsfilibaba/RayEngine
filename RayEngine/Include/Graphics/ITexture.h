#pragma once

#include "..\Defines.h"
#include "..\Types.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		struct TextureInfo
		{
			FORMAT Format = FORMAT_UNKNOWN;
			int32 Width = 0;
			int32 Height = 0;
		};



		/////////////////////////////////////////////////////////////
		class ITexture
		{
		public:
			ITexture(const ITexture& other) = delete;
			ITexture(ITexture&& other) = delete;
			ITexture& operator=(const ITexture& other) = delete;
			ITexture& operator=(ITexture&& other) = delete;

		public:
			ITexture() {}
			virtual ~ITexture() {}
		};
	}
}