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
#include "RayEngine.h"

namespace RayEngine
{
	/*////////////////////////////////////////////////////////////

		Reverses the red andb blue channel.

		pPixels - Array of containgin the pixels of the image.

		width - The width of the image in pixels.

		height - The height og the image in pixels.

		format - The format of each pixel

	////////////////////////////////////////////////////////////*/
	void RAYENGINE_API ReverseImageRedBlue(void* pPixels, int32 width, int32 height, FORMAT format);

	/*////////////////////////////////////////////////////////////

		Loads an image if it has one of the following formats:
			- PNG
			- TGA
			- BMP
			- JPG
			- HDR

		pFilename - Name of the file.

		pFilepath - Path to file relative to the executable.

		ppPixels - A valid void* that is used to store the loaded
		image.

		width - A reference to a int32 that will contain the width
		of the loaded image.

		height - A reference to a int32 that will contain the height
		of the loaded image.

		format - The format that the loaded image will contain. It 
		does not matter what format the image has on disk. The image
		will be converted to the specefied format.

		Returns true on successfull load.

	////////////////////////////////////////////////////////////*/
	bool RAYENGINE_API LoadImageFromFile(const std::string& filename, const std::string& filepath, const void** ppPixels, int32& width, int32& height, FORMAT format);
		
	/*////////////////////////////////////////////////////////////

		Resizes an image.
			
		ppPixels - A valid void* pointer that contains the pixels
		to be resized and the resulting output.

		width - The width of the image.

		height - The height of the image.

		newWidth - The new width of the image.

		newHeight - The new height of the image.

		format - The format of a pixels.

		Returns true on successfull resize.

	////////////////////////////////////////////////////////////*/
	bool RAYENGINE_API ResizeImage(void** ppPixels, int32 width, int32 height, int32 newWidth, int32 newHeight, FORMAT format);

	/*////////////////////////////////////////////////////////////

		Saves an image to disk

		pFilename - Name of the file.

		pFilepath - Path to file relative to the executable.

		pPixels - A valid pointer that is used to store the image.

		width - The width of the image.

		height - The height of the image.

		format - The format of each pixel.

		extension - The extension of the output file.

		Returns true on successfull load.

	////////////////////////////////////////////////////////////*/
	bool RAYENGINE_API SaveImageToFile(const std::string& filename, const std::string& filepath, const void* const pPixels, int32 width, int32 height, FORMAT format, IMAGE_EXTENSION extension);
}