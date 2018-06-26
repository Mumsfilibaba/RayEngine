#pragma once

#include "..\Defines.h"
#include "..\Types.h"

namespace RayEngine
{
	class RE_API BitmapImpl
	{
	public:
		BitmapImpl(const BitmapImpl& other) = delete;
		BitmapImpl(BitmapImpl&& other) = delete;
		BitmapImpl& operator=(const BitmapImpl& other) = delete;
		BitmapImpl& operator=(BitmapImpl&& other) = delete;

		BitmapImpl() {}
		virtual ~BitmapImpl() {}

		//Load a bitmap from file - See TextureLoader to see available formats - returns true if succeeded
		virtual bool LoadFromFile(const Tchar* const filename, const Tchar* filePath, int32 width, int32 height) = 0;
		//Create a bitmap from a pixelarray
		virtual bool CreateFromArray(int32 width, int32 height, const uint8* const pixels) = 0;

		//Get the actual array with pixels
		virtual uint8* GetArray() = 0;
		virtual const uint8* GetArray() const = 0;
		virtual int32 GetWidth() const = 0;
		virtual int32 GetHeight() const = 0;

		//Returns a new instance as a copy of the implementation - the old object is still valid
		virtual BitmapImpl* Copy() const = 0;
	};

	class RE_API Bitmap
	{
	public:
		Bitmap();
		Bitmap(int32 width, int32 height, const uint8* const pixels);
		Bitmap(const Tchar* const filename, const Tchar* filePath, int32 width = 0, int32 height = 0);
		Bitmap(const Bitmap& other);
		Bitmap(Bitmap&& other);
		~Bitmap();

		//Load a bitmap from file - See TextureLoader to see available formats - returns true if succeeded
		bool LoadFromFile(const Tchar* const filename, const Tchar* filePath, int32 width = 0, int32 height = 0);
		//Create a bitmap from a pixelarray
		bool CreateFromArray(int32 width, int32 height, const uint8* const pixels);

		//Get the actual array with pixels
		uint8* GetArray();
		const uint8* GetArray() const;
		int32 GetWidth() const;
		int32 GetHeight() const;

		//Get implementation interface
		const BitmapImpl* GetImplementation() const;

		Bitmap& operator=(Bitmap&& other);
		Bitmap& operator=(const Bitmap& other);

	private:
		BitmapImpl* m_Impl;
	};
}