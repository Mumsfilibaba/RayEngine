#pragma once

#include "..\Defines.h"
#include "..\Types.h"

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	enum TEX_EXTENSION : int32
	{
		TEX_EXTENSION_UNKNOWN = 0,
		TEX_EXTENSION_PNG = 1,
		TEX_EXTENSION_BMP = 2,
		TEX_EXTENSION_TGA = 3,
		TEX_EXTENSION_JPG = 4,
		TEX_EXTENSION_HDR = 5,
	};



	/////////////////////////////////////////////////////////////
	class RE_API TextureLoader
	{
	public:
		virtual ~TextureLoader() = 0;

	public:
		//Reverses the Red and Blue channels - For example converting between RGB to BGR or the other way around
		static void ReverseRB(void* pPixels, int32 width, int32 height, FORMAT format);
		//Load a texture from file - Files can be in the formats:
		//.png
		//.tga
		//.bmp
		//.hdr
		//.jpg 
		//If width or height is not zero then the image will be resized to the specifed size
		//Returns true if file loaded successfully
		static bool LoadFromFile(const Tchar* const pFilename, const Tchar* const pFilepath, const void** ppPixels, int32& width, int32& height, FORMAT format);
		//Save a texture to file - Filename should be without extension - returns true if file saved successfully
		static bool SaveToFile(const Tchar* const pFilename, const Tchar* const pFilepath, const void* const pPixels, int32 width, int32 height, FORMAT format, TEX_EXTENSION extension);
	};
}