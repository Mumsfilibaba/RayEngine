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