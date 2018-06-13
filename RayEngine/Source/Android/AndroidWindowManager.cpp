#include "AndroidWindowManager.h"

#if defined(RE_PLATFORM_ANDROID)

namespace RayEngine
{
	AndroidWindowManager::AndroidWindowManager()
		: m_Window(nullptr),
		m_Width(0),
		m_Height(0),
		m_Color(AndroidGetIntColor(255, 255, 255, 255)),
		m_Flags(0)
	{
	}

	AndroidWindowManager::~AndroidWindowManager()
	{
	}

	void AndroidWindowManager::SetFlags(ANativeActivity* activity, uint32 flags)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		SetFlagsUnsafe(activity, flags);
	}

	void AndroidWindowManager::SetColor(RayEngine::int32 color)
	{
		//Lock window
		std::lock_guard<std::mutex> lock(m_Mutex);
		SetColorUnsafe(color);
	}

	void AndroidWindowManager::SetSize(RayEngine::int32 width, RayEngine::int32 height)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		SetSizeUnsafe(width, height);
	}

	void AndroidWindowManager::OnNativeWindowChanged(ANativeActivity* activity, ANativeWindow* window, bool terminate)
	{
		//Lock
		std::lock_guard<std::mutex> lock(m_Mutex);

		//Terminate window
		if (terminate)
		{
			if (window == m_Window)
				m_Window = nullptr;
		}
		else
		{
			//Set the new windowptr
			m_Window = window;

			//Set the new window to the settings
			SetFlagsUnsafe(activity, m_Flags);
			SetSizeUnsafe(m_Width, m_Height);
			SetColorUnsafe(m_Color);
		}
	}

	void AndroidWindowManager::SetFlagsUnsafe(ANativeActivity* activity, uint32 flags)
	{
		if (activity != nullptr)
		{
			ANativeActivity_setWindowFlags(activity, flags, flags ^ (flags | m_Flags));
			m_Flags = flags;
		}
	}

	void AndroidWindowManager::SetSizeUnsafe(RayEngine::int32 width, RayEngine::int32 height)
	{
		using namespace RayEngine;

		//Return if null
		if (width == 0 || height == 0)
			return;

		//Set size
		m_Width = width;
		m_Height = height;

		//Return if null
		if (m_Window == nullptr)
			return;

		if (ANativeWindow_setBuffersGeometry(m_Window, m_Width, m_Height, 0) != 0)
		{
			LOGE("'ANativeWindow_setBuffersGeometry' failed");
		}
	}

	void AndroidWindowManager::SetColorUnsafe(RayEngine::int32 color)
	{
		using namespace RayEngine;

		//If size is zero escape
		if (m_Width == 0 || m_Height == 0)
			return;

		//Do not lock if it is zero
		if (m_Window == nullptr)
			return;

		//Set color
		m_Color = color;

		//Change color
		ARect rect = {};
		ANativeWindow_Buffer buffer = {};

		rect.bottom = m_Height;
		rect.right = m_Width;

		if (ANativeWindow_lock(m_Window, &buffer, &rect) == 0)
		{
			if (ANativeWindow_getFormat(m_Window) == WINDOW_FORMAT_RGBA_8888)
			{
				for (int32 i = (m_Width * m_Height) - 1; i >= 0; i--)
					static_cast<int32*>(buffer.bits)[i] = m_Color;
			}
			else
			{
				LOGE("'ANativeWindow_getFormat' returned unsupported format");
			}

			if (ANativeWindow_unlockAndPost(m_Window) != 0)
			{
				LOGE("'ANativeWindow_unlockAndPost' failed");
			}
		}
	}
}

#endif