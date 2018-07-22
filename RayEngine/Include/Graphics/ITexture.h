#pragma once

#include "..\Defines.h"
#include "..\Types.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		enum TEXTURETYPE : int32
		{
			TEXTURETYPE_UNKNOWN = 0,
			TEXTURETYPE_1D = 1,
			TEXTURETYPE_2D = 2,
			TEXTURETYPE_3D = 3,
		};



		/////////////////////////////////////////////////////////////
		enum TEXTUREFLAGS : int32
		{
			TEXTUREFLAGS_NONE = 0,
			TEXTUREFLAGS_RENDERTARGET = 1,
			TEXTUREFLAGS_TEXTURE = 2,
			TEXTUREFLAGS_DEPTHBUFFER = 3,
		};



		/////////////////////////////////////////////////////////////
		struct TextureInfo
		{
			FORMAT Format = FORMAT_UNKNOWN;
			TEXTURETYPE TextureType = TEXTURETYPE_2D;
			TEXTUREFLAGS Flags = TEXTUREFLAGS_NONE;
			RESOURCE_USAGE Usage = RESOURCE_USAGE_UNKNOWN;
			int32 Width = 0;
			int32 Height = 0;
			int32 DepthOrArraySize = 0;
			int32 SampleCount = 1;
			int32 MipLevels = 1;
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