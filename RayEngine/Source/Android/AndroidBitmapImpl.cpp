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

#include "..\..\Include\Android\AndroidBitmapImpl.h"

#if defined(RE_PLATFORM_ANDROID)
#include <cstring>

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	AndroidBitmapImpl::AndroidBitmapImpl()
		: m_Width(0),
		m_Height(0),
		m_Pixels(nullptr)
	{
	}



	/////////////////////////////////////////////////////////////
	AndroidBitmapImpl::~AndroidBitmapImpl()
	{
		if (m_Pixels != nullptr)
		{
			delete[] m_Pixels;
			m_Pixels = nullptr;
		}
	}



	/////////////////////////////////////////////////////////////
	bool AndroidBitmapImpl::LoadFromFile(const Tchar * const filename, const Tchar * filePath, int32 width, int32 height)
	{
		return false;
	}



	/////////////////////////////////////////////////////////////
	bool AndroidBitmapImpl::CreateFromArray(int32 width, int32 height, const uint8 * const pixels)
	{
		return false;
	}



	/////////////////////////////////////////////////////////////
	uint8* AndroidBitmapImpl::GetArray()
	{
		return m_Pixels;
	}



	/////////////////////////////////////////////////////////////
	const uint8 * AndroidBitmapImpl::GetArray() const
	{
		return m_Pixels;
	}



	/////////////////////////////////////////////////////////////
	int32 AndroidBitmapImpl::GetWidth() const
	{
		return m_Width;
	}



	/////////////////////////////////////////////////////////////
	int32 AndroidBitmapImpl::GetHeight() const
	{
		return m_Height;
	}



	/////////////////////////////////////////////////////////////
	IBitmapImpl* AndroidBitmapImpl::Copy() const
	{
		AndroidBitmapImpl* impl = new AndroidBitmapImpl();
		impl->m_Height = m_Height;
		impl->m_Width = m_Width;

		const int32 bytes = m_Width * m_Height;
		impl->m_Pixels = new uint8[bytes];
		memcpy(impl->m_Pixels, m_Pixels, bytes);

		return impl;
	}
}

#endif