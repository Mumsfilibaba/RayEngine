#pragma once

#include "..\..\Include\System\Bitmap.h"

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