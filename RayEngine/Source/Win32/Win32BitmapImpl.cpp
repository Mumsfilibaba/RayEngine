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

#include <cstdlib>
#include "..\..\Include\Utilities\TextureUtilities.h"
#include "..\..\Include\Win32\Win32BitmapImpl.h"

#if defined (RE_PLATFORM_WINDOWS)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HICON CreateIcon(const RayEngine::Win32BitmapImpl* const bitmap)
{
	ICONINFO info = {};
	info.fIcon = TRUE;
	info.hbmColor = bitmap->GetBitmap();
	info.hbmMask = bitmap->GetMask();

	return (info.hbmColor == 0 || info.hbmMask == 0) ? 0 : CreateIconIndirect(&info);
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HCURSOR CreateCursor(const RayEngine::Win32BitmapImpl* const bitmap, RayEngine::int32 hotspotX, RayEngine::int32 hotspotY)
{
	ICONINFO info = {};
	info.fIcon = FALSE;
	info.xHotspot = hotspotX;
	info.yHotspot = hotspotY;
	info.hbmColor = bitmap->GetBitmap();
	info.hbmMask = bitmap->GetMask();

	return (info.hbmColor == 0 || info.hbmMask == 0) ? 0 : reinterpret_cast<HCURSOR>(CreateIconIndirect(&info));
}



namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Win32BitmapImpl::Win32BitmapImpl()
		: m_Hmask(0),
		m_Hbitmap(0),
		m_Pixels(nullptr),
		m_Width(0),
		m_Height(0)
	{
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Win32BitmapImpl::~Win32BitmapImpl()
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

		if (m_Pixels != nullptr)
		{
			delete m_Pixels;
			m_Pixels = nullptr;
		}
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Win32BitmapImpl::LoadFromFile(const Tchar* const filename, const Tchar* filepath, int32 width, int32 height)
	{
		if (m_Pixels != nullptr)
		{
			delete[] m_Pixels;
			m_Pixels = nullptr;
		}

		m_Width = width;
		m_Height = height;

		if (LoadImageFromFile(filename, filepath, const_cast<const void**>(reinterpret_cast<void**>(&m_Pixels)),
			m_Width, m_Height, FORMAT_R8G8B8A8_UINT))
		{
			ReverseImageRedBlue(m_Pixels, m_Width, m_Height, FORMAT_R8G8B8A8_UINT);

			Create();

			return true;
		}

		m_Width = 0;
		m_Height = 0;

		return false;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Win32BitmapImpl::CreateFromArray(int32 width, int32 height, const uint8* const pixels)
	{
		if (m_Pixels != nullptr)
		{
			delete[] m_Pixels;
			m_Pixels = nullptr;
		}

		m_Width = width;
		m_Height = height;

		m_Pixels = reinterpret_cast<uint8*>(malloc(m_Width * m_Height * 4));
		memcpy(m_Pixels, pixels, m_Width * m_Height * 4);

		Create();

		return true;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	uint8* Win32BitmapImpl::GetArray()
	{
		return m_Pixels;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const uint8* Win32BitmapImpl::GetArray() const
	{
		return m_Pixels;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int32 Win32BitmapImpl::GetWidth() const
	{
		return m_Width;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int32 Win32BitmapImpl::GetHeight() const
	{
		return m_Height;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	HBITMAP Win32BitmapImpl::GetMask() const
	{
		return m_Hmask;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	HBITMAP Win32BitmapImpl::GetBitmap() const
	{
		return m_Hbitmap;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IBitmapImpl* Win32BitmapImpl::Copy() const
	{
		Win32BitmapImpl* res = new Win32BitmapImpl();
		res->m_Hmask = reinterpret_cast<HBITMAP>(CopyImage(m_Hmask, IMAGE_BITMAP, 0, 0, 0));
		res->m_Hbitmap = reinterpret_cast<HBITMAP>(CopyImage(m_Hbitmap, IMAGE_BITMAP, 0, 0, 0));
		res->m_Pixels = reinterpret_cast<uint8*>(malloc(m_Width * m_Height * 4));
		memcpy(res->m_Pixels, m_Pixels, m_Width * m_Height * 4);

		return res;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Win32BitmapImpl::Create()
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

		m_Hmask = CreateBitmap(m_Width, m_Height, 1, 1, 0);
		m_Hbitmap = CreateBitmap(m_Width, m_Height, 1, 32, reinterpret_cast<const void*>(m_Pixels));
	}
}

#endif