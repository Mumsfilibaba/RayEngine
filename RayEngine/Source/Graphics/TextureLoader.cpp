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

#include "..\..\Include\Graphics\TextureLoader.h"

#define STBI_NO_PSD
#define STBI_NO_GIF
#define STBI_NO_PIC
#define STBI_NO_PNM
#if !defined(RE_PLATFORM_ANDROID)
#define STBI_MSC_SECURE_CRT
#else
#define STBI_NO_SIMD
#endif
#define STB_IMAGE_IMPLEMENTATION
#include "..\..\Dependencies\stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "..\..\Dependencies\stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "..\..\Dependencies\stb_image_resize.h"

#include <string>
#if defined(UNICODE)
typedef std::basic_string<wchar_t> Tstring;
#else
typedef std::basic_string<char> Tstring;
#endif

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	void TextureLoader::ReverseRB(void* pixels, int32 width, int32 height, FORMAT format)
	{
		if (format == FORMAT_R8G8B8A8_UINT)
		{
			uint8* data = static_cast<uint8*>(pixels);

			//TODO: Different loops based on components

			uint8 temp = 0;
			for (int32 i = (width * height * 4) - 1; i >= 0; i -= 4)
			{
				temp = data[i - 1];
				data[i - 1] = data[i - 3];
				data[i - 3] = temp;
			}
		}
		else if (format == FORMAT_R32G32B32A32_FLOAT)
		{
			float* data = static_cast<float*>(pixels);

			//TODO: Different loops based on components

			float temp = 0;
			for (int32 i = (width * height * 4) - 1; i >= 0; i -= 4)
			{
				temp = data[i - 1];
				data[i - 1] = data[i - 3];
				data[i - 3] = temp;
			}
		}
	}



	/////////////////////////////////////////////////////////////
	bool TextureLoader::LoadFromFile(const Tchar* const filename, const Tchar* const filepath, 
		const void** pixels, int32& width, int32& height, FORMAT format)
	{
		if (pixels == nullptr || format == FORMAT_UNKNOWN)
			return false;

		Tstring fullpath = filepath + Tstring(filename);
		int32 wi = 0;
		int32 he = 0;
		int32 reqComponents = 0;
		void* data = nullptr;

		if (format == FORMAT_R8G8B8A8_UINT || format == FORMAT_R8G8B8A8_UNORM)
			data = static_cast<void*>(stbi_load(fullpath.c_str(), &wi, &he, &reqComponents, 4));
		else if (format == FORMAT_R32G32B32A32_FLOAT)
			data = stbi_loadf(fullpath.c_str(), &wi, &he, &reqComponents, 4);

		if (data != nullptr)
		{
			//Resize image if wanted
			if (width != 0 || height != 0)
			{
				int32 outWi = (width == 0) ? wi : width;
				int32 outHe = (height == 0) ? he : height;
				int32 res = 0;
				void* output = nullptr;
				
				if (format == FORMAT_R8G8B8A8_UINT || format == FORMAT_R8G8B8A8_UNORM)
				{
					output = new uint8[outWi * outHe * 4];
					res = stbir_resize_uint8(reinterpret_cast<const uint8*>(data), wi, he, 0,
						reinterpret_cast<uint8*>(output), outWi, outHe, 0, 4);

					delete[] reinterpret_cast<uint8*>(data);
				}
				else if (format == FORMAT_R32G32B32A32_FLOAT)
				{
					output = new float[outWi * outHe * 4];
					stbir_resize_float(reinterpret_cast<const float*>(data), wi, he, 0,
						reinterpret_cast<float*>(output), outWi, outHe, 0, 4);

					delete[] reinterpret_cast<float*>(data);
				}
				
				data = nullptr;

				if (res != 0)
				{
					(*pixels) = static_cast<const void*>(output);
					wi = outWi;
					he = outHe;
				}
			}
			else
			{
				width = wi;
				height = he;
				(*pixels) = data;
			}
			
			return true;
		}

		const char* reason = stbi_failure_reason();

		height = 0;
		width = 0;
		(*pixels) = nullptr;
		return false;
	}



	/////////////////////////////////////////////////////////////
	bool TextureLoader::SaveToFile(const Tchar* const filename, const Tchar* const filepath, 
		const void* const pixels, int32 width, int32 height, FORMAT format, TEX_EXTENSION extension)
	{
		if (pixels == nullptr || width == 0 || height == 0 || format == FORMAT_UNKNOWN || extension == TEX_EXTENSION_UNKNOWN)
			return false;

		Tstring fullpath = filepath + Tstring(filename);
		if (extension == TEX_EXTENSION_HDR)
		{
			return false;

			//TODO: Fix saving to .hdr

			//TODO: Do ldr to hdr
			if (format != FORMAT_R32G32B32A32_FLOAT)
				return false;

			fullpath += RE_T(".hdr");
			return (stbi_write_hdr(fullpath.c_str(), width, height, 4, static_cast<const float*>(pixels)) != 0);
		}
		else 
		{
			//TODO: Do hdr to ldr
			if (format != FORMAT_R8G8B8A8_UINT)
				return false;

			int32 stride = (sizeof(uint8) * 4) * width;
			if (extension == TEX_EXTENSION_PNG)
			{
				fullpath += RE_T(".png");
				return (stbi_write_png(fullpath.c_str(), width, height, 4, pixels, stride) != 0);
			}
			else if (extension == TEX_EXTENSION_BMP)
			{
				fullpath += RE_T(".bmp");
				return (stbi_write_bmp(fullpath.c_str(), width, height, 4, pixels) != 0);
			}
			else if (extension == TEX_EXTENSION_JPG)
			{
				fullpath += RE_T(".jpg");
				return (stbi_write_jpg(fullpath.c_str(), width, height, 4, pixels, 50) != 0);
			}
			else if (extension == TEX_EXTENSION_TGA)
			{
				fullpath += RE_T(".tga");
				return (stbi_write_tga(fullpath.c_str(), width, height, 4, pixels) != 0);
			}
		}

		return false;
	}
}