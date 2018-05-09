#pragma once
#include "..\..\Include\Types.h"
#include "..\..\Include\Defines.h"
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

namespace RayEngine
{
	//Class for keeping 32-Bit Bitmaps and Mask for rendering the whole bitmask
	class Win32Bitmap
	{
	public:
		Win32Bitmap(int32 width, int32 height, const uint8* pixels);
		//A width and height of zero means that the bitmap ses the size of the loaded image
		//Otherwise the image will be resized to the specied one
		Win32Bitmap(const Tchar* filename, const Tchar* filepath, int32 width = 0, int32 height = 0);
		Win32Bitmap(const Win32Bitmap& other);
		Win32Bitmap(Win32Bitmap&& other);
		~Win32Bitmap();

		bool IsValid() const;

		HBITMAP GetMask() const;
		HBITMAP GetBitmap() const;

		Win32Bitmap& operator=(const Win32Bitmap& other);
		Win32Bitmap& operator=(Win32Bitmap&& other);

	private:
		void Create(int32 width, int32 height, const uint8* pixels);

	private:
		HBITMAP m_Hmask;
		HBITMAP m_Hbitmap;
	};
}