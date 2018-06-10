#include "..\..\Include\System\Window.h"
#include <cassert>

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\Win32\Win32WindowImpl.h"
#elif defined(RE_PLATFORM_ANDROID)
#include "..\..\Include\Android\AndroidWindowImpl.h"
#else
#error RayEngine needs to have a platform defined
#endif

namespace RayEngine
{
	namespace System
	{
		Window::Window(const WindowDesc& desc)
			: m_Impl(nullptr)
		{
#if defined(RE_PLATFORM_WINDOWS)
			m_Impl = new Win32WindowImpl();
#elif defined(RE_PLATFORM_ANDROID)
			m_Impl = new AndroidWindowImpl();
#endif
			if (m_Impl != nullptr)
				m_Impl->Create(desc);
		}

		Window::Window(const Window& other)
			: m_Impl(nullptr)
		{
			assert(other.m_Impl != nullptr);
			m_Impl = other.m_Impl->Copy();
		}

		Window::Window(Window&& other)
			: m_Impl(other.m_Impl)
		{
			//Set other so that it is no longer valid
			other.m_Impl = nullptr;
		}

		Window::~Window()
		{
			delete m_Impl;
			m_Impl = nullptr;
		}

		void Window::Show() const
		{
			assert(m_Impl != nullptr);
			m_Impl->Show();
		}

		bool Window::PeekEvent(Event& pEvent) const
		{
			assert(m_Impl != nullptr);

			m_Impl->PeekEvents();
			return m_Impl->PopEvent(pEvent);
		}

		void Window::GetEvent(Event& pEvent) const
		{
			assert(m_Impl != nullptr);
			m_Impl->GetEvent(pEvent);
		}

		void Window::SendQuitEvent(int32 exitCode) const
		{
			assert(m_Impl != nullptr);
			m_Impl->SendQuitEvent(exitCode);
		}

		void Window::SetCursor(const Cursor& cursor)
		{
			assert(m_Impl != nullptr);
			m_Impl->SetCursor(cursor);
		}

		void Window::SetIcon(const Icon& icon)
		{
			assert(m_Impl != nullptr);
			m_Impl->SetIcon(icon);
		}

		void Window::SetBackground(uint8 r, uint8 g, uint8 b)
		{
			assert(m_Impl != nullptr);
			m_Impl->SetBackground(r, g, b);
		}

		int32 Window::GetWidth() const
		{
			assert(m_Impl != nullptr);
			return m_Impl->GetWidth();
		}

		int32 Window::GetHeight() const
		{
			assert(m_Impl != nullptr);
			return m_Impl->GetHeight();
		}

		const Tchar* Window::GetTitle() const
		{
			assert(m_Impl != nullptr);
			return m_Impl->GetTitle();
		}

		const IWindowImpl* Window::GetImplementation() const
		{
			return m_Impl;
		}

		void Window::GetDesc(WindowDesc& desc) const
		{
			assert(m_Impl != nullptr);
			m_Impl->GetDesc(desc);
		}

		Window& Window::operator=(const Window& other)
		{
			if (this != &other)
			{
				delete m_Impl;
				m_Impl = (other.m_Impl != nullptr) ? other.m_Impl->Copy() : nullptr;
			}
			
			return *this;
		}

		Window& Window::operator=(Window&& other)
		{
			if (this != &other)
			{
				delete m_Impl;
				m_Impl = other.m_Impl;

				other.m_Impl = nullptr;
			}

			return *this;
		}
	}
}