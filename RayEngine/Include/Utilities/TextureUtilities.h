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
	/*////////////////////////////////////////////////////////////
		
		Revereses the Red and blue channel of the texture. This
		function can be used to convert between RGB and BRG or 
		the other way around.

		pPixels - Array of pixels

		width - Width of the image in pixels

		height - Height of the image in pixels

		format - Format of each pixel

	////////////////////////////////////////////////////////////*/
	void ReverseRedBlue(void* pPixels, int32 width, int32 height, FORMAT format);
	
	
	/*////////////////////////////////////////////////////////////
		
		Loads and image from file. Files can have the following
		formats:
		png
		tga
		bmp
		hdr
		jpg

		If width or height is not equal to zero the image will
		be resized after successfull loading to the specified 
		values.

		Returns true if the image is loaded successfully.

		pFilename - Name of the file, with fileextension.

		pFilepath - The path of the file. Can be nullptr, that
		will give the same result as specifing an empty string.

		ppPixels - A pointer to an array. The pointer sent in
		will contain the image after a successfull call. The
		type is based on the format sent in. E.g if format equals
		FORMAT_R16G16B16A16_FLOAT then the resulting type is
		an array of float16.

		width - The width of the loaded image. If this is non-zero
		when called the resulting image will have the same width
		as the specified value.

		height - The height of the loaded image. If this is non-zero
		when called the resulting image will have the same height
		as the specified value.

		format - The format that the loaded image will have.

	////////////////////////////////////////////////////////////*/
	bool LoadFromFile(const Tchar* const pFilename, const Tchar* const pFilepath, const void** ppPixels, int32& width, int32& height, FORMAT format);
	
	
	/*////////////////////////////////////////////////////////////

		Saves an array of pixels to a file. Returns true on 
		successfull save.

		pFilename - Name of the file, should not contain any extension.

		pFilepath - The path of the file. Can be nullptr, that
		will give the same result as specifing an empty string.

		pPixels - Array of pixels

		width - Width of the image in pixels

		height - Height of the image in pixels

		format - Format of each pixel

		extension - Extension of the file. This basically is
		what format the outputed image will have. E.g
		IMAGE_EXTENSION_PNG results in an .png-image.

	////////////////////////////////////////////////////////////*/
	bool SaveToFile(const Tchar* const pFilename, const Tchar* const pFilepath, const void* const pPixels, int32 width, int32 height, FORMAT format, IMAGE_EXTENSION extension);
}