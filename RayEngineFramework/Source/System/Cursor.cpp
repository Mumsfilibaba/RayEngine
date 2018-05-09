#include "..\..\Include\System\Cursor.h"
#include <cassert>

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\Win32\Win32CursorImpl.h"
#else
#error RayEngine needs a platform to be defined
#endif

namespace RayEngine
{
	namespace System
	{
		Cursor::Cursor()
			: m_Impl(nullptr)
		{
#if defined(RE_PLATFORM_WINDOWS)
			m_Impl = new Win32CursorImpl();
#endif
		}

		Cursor::Cursor(const Cursor& other)
			: m_Impl(nullptr)
		{
			if (other.m_Impl != nullptr)
				m_Impl = other.m_Impl->Copy();
		}

		Cursor::Cursor(Cursor&& other)
			: m_Impl(other.m_Impl)
		{
			other.m_Impl = nullptr;
		}

		Cursor::~Cursor()
		{
			if (m_Impl != nullptr)
			{
				delete m_Impl;
				m_Impl = nullptr;
			}
		}

		bool Cursor::LoadFromFile(const Tchar* filename, const Tchar* filepath, int32 x, int32 y)
		{
			assert(m_Impl != nullptr);
			return m_Impl->LoadFromFile(filename, filepath, x, y);
		}

		bool Cursor::LoadFromOS(STANDARD_CURSOR cursor)
		{
			assert(m_Impl != nullptr);
			return m_Impl->LoadFromOS(cursor);
		}

		const CursorImpl* Cursor::GetImplementation() const
		{
			return m_Impl;
		}

		Cursor& Cursor::operator=(const Cursor& other)
		{
			if (this != &other)
			{
				if (m_Impl != nullptr)
					delete m_Impl;

				m_Impl = (other.m_Impl != nullptr) ? other.m_Impl->Copy() : nullptr;
			}

			return *this;
		}

		Cursor& Cursor::operator=(Cursor&& other)
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
	}
}