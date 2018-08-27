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
#include "..\..\Include\System\Bitmap.h"
#if defined (RE_PLATFORM_WINDOWS)

#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	class RE_API Win32BitmapImpl : public IBitmapImpl
	{
	public:
		Win32BitmapImpl& operator=(Win32BitmapImpl&& other) = delete;
		Win32BitmapImpl& operator=(const Win32BitmapImpl& other) = delete;
		Win32BitmapImpl(Win32BitmapImpl&& other) = delete;
		Win32BitmapImpl(const Win32BitmapImpl& other) = delete;

		Win32BitmapImpl();
		~Win32BitmapImpl();

		bool LoadImageFromFile(const Tchar* const filename, const Tchar* filePath, int32 width, int32 height) override final;
		bool CreateFromArray(int32 width, int32 height, const uint8* const pixels) override final;

		uint8* GetArray() override final;
		const uint8* GetArray() const override final;
		int32 GetWidth() const override final;
		int32 GetHeight() const override final;

		IBitmapImpl* Copy() const override final;

		HBITMAP GetMask() const;
		HBITMAP GetBitmap() const;

	private:
		void Create();

	private:
		HBITMAP m_Hmask;
		HBITMAP m_Hbitmap;
		uint8* m_Pixels;
		int32 m_Width;
		int32 m_Height;
	};
}



/////////////////////////////////////////////////////////////
HICON CreateIcon(const RayEngine::Win32BitmapImpl* const bitmap);
HCURSOR CreateCursor(const RayEngine::Win32BitmapImpl* const bitmap, RayEngine::int32 hotspotX, RayEngine::int32 hotspotY);

#endif