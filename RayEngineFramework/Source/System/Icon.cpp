#include "..\..\Include\System\Icon.h"
#include <cassert>

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\Win32\Win32IconImpl.h"
#else
#error RayEngine needs a platform to be defined
#endif

namespace RayEngine
{
	namespace System
	{
		Icon::Icon()
			: m_Impl(nullptr)
		{
#if defined(RE_PLATFORM_WINDOWS)
			m_Impl = new Win32IconImpl();
#endif
		}

		Icon::~Icon()
		{
			if (m_Impl != nullptr)
			{
				delete m_Impl;
				m_Impl = nullptr;
			}
		}

		Icon::Icon(const Icon& other)
			: m_Impl(nullptr)
		{
			if (other.m_Impl != nullptr)
				m_Impl = other.m_Impl->Copy();
			else
				m_Impl = nullptr;
		}

		Icon::Icon(Icon&& other)
			: m_Impl(nullptr)
		{
			m_Impl = other.m_Impl;
			other.m_Impl = nullptr;
		}

		bool Icon::LoadFromFile(const Tchar* const filename, const Tchar* const filepath)
		{
			assert(m_Impl != nullptr);
			return m_Impl->LoadFromFile(filename, filepath);
		}

		bool Icon::LoadFromOS(STANDARD_ICON icon)
		{
			assert(m_Impl != nullptr);
			return m_Impl->LoadFromOS(icon);
		}

		const IconImpl* Icon::GetImplementation() const
		{
			return m_Impl;
		}

		Icon& Icon::operator=(const Icon& other)
		{
			if (this != &other)
			{
				if (m_Impl != nullptr)
					delete m_Impl;

				if (other.m_Impl != nullptr)
					m_Impl = other.m_Impl->Copy();
			}

			return *this;
		}

		Icon& Icon::operator=(Icon&& other)
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