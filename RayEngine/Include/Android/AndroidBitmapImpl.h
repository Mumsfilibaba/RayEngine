#pragma once

#include "..\..\Include\System\Bitmap.h"

#if defined(RE_PLATFORM_ANDROID)

namespace RayEngine
{
	class AndroidBitmapImpl : public IBitmapImpl
	{
	public:
		AndroidBitmapImpl& operator=(AndroidBitmapImpl&& other) = delete;
		AndroidBitmapImpl& operator=(const AndroidBitmapImpl& other) = delete;
		AndroidBitmapImpl(AndroidBitmapImpl&& other) = delete;
		AndroidBitmapImpl(const AndroidBitmapImpl& other) = delete;

		AndroidBitmapImpl();
		~AndroidBitmapImpl();

		bool LoadFromFile(const Tchar* const filename, const Tchar* filePath, int32 width, int32 height) override final;
		bool CreateFromArray(int32 width, int32 height, const uint8* const pixels) override final;

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