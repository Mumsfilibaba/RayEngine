#pragma once

#include "..\..\Include\Defines.h"

#if defined(RE_PLATFORM_ANDROID)

#include "..\..\Include\Types.h"
#include "..\..\Include\Android\Android.h"

#include <mutex>
#include <android\native_window.h>

namespace RayEngine
{
	class AndroidWindowManager
	{
	public:
		AndroidWindowManager(AndroidWindowManager&& other) = delete;
		AndroidWindowManager(const AndroidWindowManager& other) = delete;
		AndroidWindowManager& operator=(AndroidWindowManager&& other) = delete;
		AndroidWindowManager& operator=(const AndroidWindowManager& other) = delete;

		AndroidWindowManager();
		~AndroidWindowManager();

		void SetFlags(ANativeActivity* activity, uint32 flags);
		void SetColor(RayEngine::int32 color);
		void SetSize(RayEngine::int32 width, RayEngine::int32 height);
		void OnNativeWindowChanged(ANativeActivity* activity, ANativeWindow* window, bool terminate);

	private:
		void SetFlagsUnsafe(ANativeActivity* activity, uint32 flags);
		void SetSizeUnsafe(RayEngine::int32 width, RayEngine::int32 height);
		void SetColorUnsafe(RayEngine::int32 color);

	private:
		std::mutex m_Mutex;
		ANativeWindow* m_Window;

		int32 m_Color;
		int32 m_Height;
		int32 m_Width;

		uint32 m_Flags;
	};
}

#endif