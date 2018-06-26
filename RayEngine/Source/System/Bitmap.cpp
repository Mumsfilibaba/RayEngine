#include "..\..\Include\System\Bitmap.h"
#include <cassert>

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\Win32\Win32BitmapImpl.h"
#elif defined(RE_PLATFORM_ANDROID)
#else
#error RayEngine needs a platform to be defined
#endif

#if defined(RE_PLATFORM_WINDOWS)
#define NewBitmapImpl(...) new Win32BitmapImpl(__VA_ARGS__)
#elif defined(RE_PLATFORM_ANDROID)
#define NewBitmapImpl(...)
#else
#error RayEngine needs a platform to be defined
#endif

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	Bitmap::Bitmap()
		: m_Impl(nullptr)
	{
		m_Impl = NewBitmapImpl();
	}



	/////////////////////////////////////////////////////////////
	Bitmap::Bitmap(int32 width, int32 height, const uint8* const pixels)
		: m_Impl(nullptr)
	{
		m_Impl = NewBitmapImpl();
		m_Impl->CreateFromArray(width, height, pixels);
	}



	/////////////////////////////////////////////////////////////
	Bitmap::Bitmap(const Tchar* const filename, const Tchar* filepath, int32 width, int32 height)
		: m_Impl(nullptr)
	{
		m_Impl = NewBitmapImpl();
		m_Impl->LoadFromFile(filename, filepath, width, height);
	}



	/////////////////////////////////////////////////////////////
	Bitmap::Bitmap(const Bitmap& other)
		: m_Impl(nullptr)
	{
		if (other.m_Impl != nullptr)
			m_Impl = other.m_Impl->Copy();
	}



	/////////////////////////////////////////////////////////////
	Bitmap::Bitmap(Bitmap&& other)
		: m_Impl(other.m_Impl)
	{
		other.m_Impl = nullptr;
	}



	/////////////////////////////////////////////////////////////
	Bitmap::~Bitmap()
	{
		if (m_Impl != nullptr)
		{
			delete m_Impl;
			m_Impl = nullptr;
		}
	}



	/////////////////////////////////////////////////////////////
	bool Bitmap::LoadFromFile(const Tchar* const filename, const Tchar* filePath, int32 width, int32 height)
	{
		assert(m_Impl != nullptr);
		return m_Impl->LoadFromFile(filename, filePath, width, height);
	}



	/////////////////////////////////////////////////////////////
	bool Bitmap::CreateFromArray(int32 width, int32 height, const uint8* const pixels)
	{
		return false;
	}



	/////////////////////////////////////////////////////////////
	uint8* Bitmap::GetArray()
	{
		assert(m_Impl != nullptr);
		return m_Impl->GetArray();
	}



	/////////////////////////////////////////////////////////////
	const uint8* Bitmap::GetArray() const
	{
		assert(m_Impl != nullptr);
		return m_Impl->GetArray();
	}



	/////////////////////////////////////////////////////////////
	int32 Bitmap::GetWidth() const
	{
		assert(m_Impl != nullptr);
		return m_Impl->GetWidth();
	}



	/////////////////////////////////////////////////////////////
	int32 Bitmap::GetHeight() const
	{
		assert(m_Impl != nullptr);
		return m_Impl->GetHeight();
	}



	/////////////////////////////////////////////////////////////
	const BitmapImpl* Bitmap::GetImplementation() const
	{
		return m_Impl;
	}



	/////////////////////////////////////////////////////////////
	Bitmap& Bitmap::operator=(Bitmap&& other)
	{
		if (this != &other)
		{
			if (m_Impl != nullptr)
				delete m_Impl;

			m_Impl = other.m_Impl;
			other.m_Impl = nullptr;
		}

		return *this;
	}



	/////////////////////////////////////////////////////////////
	Bitmap& Bitmap::operator=(const Bitmap& other)
	{
		if (this != &other)
		{
			if (m_Impl != nullptr)
				delete m_Impl;

			m_Impl = (other.m_Impl != nullptr) ? other.m_Impl->Copy() : nullptr;
		}

		return *this;
	}
}