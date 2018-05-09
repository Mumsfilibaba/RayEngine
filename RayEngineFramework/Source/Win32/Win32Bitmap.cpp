#include "..\..\Include\Graphics\TextureLoader.h"
#include "Win32Bitmap.h"

namespace RayEngine
{
	Win32Bitmap::Win32Bitmap(int32 width, int32 height, const uint8* pixels)
		: m_Hmask(0),
		m_Hbitmap(0)
	{
		Create(width, height, pixels);
	}

	Win32Bitmap::Win32Bitmap(const Tchar* filename, const Tchar* filepath, int32 width, int32 height)
		: m_Hmask(0),
		m_Hbitmap(0)
	{
		int32 wi = width;
		int32 he = height;
		uint8* pixels = nullptr;

		//Load image
		if (TextureLoader::LoadFromFile(filename, filepath, const_cast<const void**>(reinterpret_cast<void**>(&pixels)), wi, he, FORMAT_R8G8B8A8_UINT))
		{
			//Convert RGB to BGR
			TextureLoader::ReverseRB(pixels, wi, he, FORMAT_R8G8B8A8_UINT);

			//Create bitmaps
			Create(wi, he, pixels);

			//Delete pixels
			delete[] pixels;
			pixels = nullptr;
		}
	}

	Win32Bitmap::Win32Bitmap(const Win32Bitmap& other)
		: m_Hmask(0),
		m_Hbitmap(0)
	{
		m_Hmask = reinterpret_cast<HBITMAP>(CopyImage(other.m_Hmask, IMAGE_BITMAP, 0, 0, 0));
		m_Hbitmap = reinterpret_cast<HBITMAP>(CopyImage(other.m_Hbitmap, IMAGE_BITMAP, 0, 0, 0));
	}

	Win32Bitmap::Win32Bitmap(Win32Bitmap&& other)
		: m_Hmask(other.m_Hmask),
		m_Hbitmap(other.m_Hbitmap)
	{
		other.m_Hmask = 0;
		other.m_Hbitmap = 0;
	}

	Win32Bitmap::~Win32Bitmap()
	{
		if (m_Hmask != 0)
		{
			DeleteObject(static_cast<HGDIOBJ>(m_Hmask));
			m_Hmask = 0;
		}

		if (m_Hbitmap != 0)
		{
			DeleteObject(static_cast<HGDIOBJ>(m_Hbitmap));
			m_Hbitmap = 0;
		}
	}

	bool Win32Bitmap::IsValid() const
	{
		return (m_Hmask != 0) && (m_Hbitmap != 0);
	}

	HBITMAP Win32Bitmap::GetMask() const
	{
		return m_Hmask;
	}

	HBITMAP Win32Bitmap::GetBitmap() const
	{
		return m_Hbitmap;
	}

	Win32Bitmap& Win32Bitmap::operator=(const Win32Bitmap& other)
	{
		if (this != &other)
		{
			m_Hmask = reinterpret_cast<HBITMAP>(CopyImage(other.m_Hmask, IMAGE_BITMAP, 0, 0, 0));
			m_Hbitmap = reinterpret_cast<HBITMAP>(CopyImage(other.m_Hbitmap, IMAGE_BITMAP, 0, 0, 0));
		}

		return *this;
	}

	Win32Bitmap& Win32Bitmap::operator=(Win32Bitmap&& other)
	{
		if (this != &other)
		{
			m_Hmask = other.m_Hmask;
			m_Hbitmap = other.m_Hbitmap;

			other.m_Hmask = 0;
			other.m_Hbitmap = 0;
		}

		return *this;
	}

	void Win32Bitmap::Create(int32 width, int32 height, const uint8* pixels)
	{
		//Create bitmap from image (Color Bit-Mask)
		m_Hbitmap = CreateBitmap(width, height, 1, 32, reinterpret_cast<const void*>(pixels));

		//Create mask
		m_Hmask = CreateBitmap(width, height, 1, 1, 0);
	}
}