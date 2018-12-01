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
#include "../../Include/Debug/Debug.h"
//#include "..\..\Include\System\Bitmap.h"

#if defined(RE_PLATFORM_ANDROID)

namespace RayEngine
{
	class AndroidBitmapImpl : public IBitmapImpl
	{
	public:
		AndroidBitmapImpl& operator=(AndroidBitmapImpl&& rrOther) = delete;
		AndroidBitmapImpl& operator=(const AndroidBitmapImpl& rOther) = delete;
		AndroidBitmapImpl(AndroidBitmapImpl&& rrOther) = delete;
		AndroidBitmapImpl(const AndroidBitmapImpl& rOther) = delete;

	public:
		AndroidBitmapImpl();
		~AndroidBitmapImpl();

		bool LoadFromFile(const Tchar* const pFilename, const Tchar* pFilePath, int32 width, int32 height) override final;
		bool CreateFromArray(int32 width, int32 height, const uint8* const pPixels) override final;

		uint8* GetArray() override final;
		const uint8* GetArray() const override final;
		int32 GetWidth() const override final;
		int32 GetHeight() const override final;

		IBitmapImpl* Copy() const override final;

	private:
		uint8* m_Pixels;
		int32 m_Width;
		int32 m_Height;
	};
}

#endif