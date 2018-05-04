#include "..\..\Include\System\Window.h"
#include <cassert>

namespace RayEngine
{
	namespace System
	{
		Window::Window(const TChar* title, int32 width, int32 height)
			: m_Width(0),
			m_Height(0),
			m_Impl(nullptr)
		{
			//TODO: Create implementation here
		}

		Window::Window(const Window& other)
			: m_Width(0),
			m_Height(0),
			m_Impl(nullptr)
		{
			//TODO: Copy implementation here
		}

		Window::Window(Window&& other)
			: m_Width(other.m_Width),
			m_Height(other.m_Height),
			m_Impl(other.m_Impl)
		{
			//Set other so that it is no longer valid
			other.m_Impl = nullptr;
			other.m_Width = 0;
			other.m_Height = 0;
		}

		Window::~Window()
		{
			delete m_Impl;
			m_Impl = nullptr;
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

		int32 Window::GetWidth() const
		{
			return m_Width;
		}

		int32 Window::GetHeight() const
		{
			return m_Height;
		}

		const TChar* Window::GetTitle() const
		{
			return nullptr;
		}
	}
}