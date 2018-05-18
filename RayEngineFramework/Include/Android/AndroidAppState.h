#pragma once

#include "..\Defines.h"
#if defined(RE_PLATFORM_ANDROID)
#include "..\Types.h"
#include <android/native_window.h>
#include <android/native_activity.h>

void ANativeActivity_createFunc(ANativeActivity* activity, void* savedState, size_t savedStateSize);

namespace RayEngine
{
	class SavedState
	{
	public:
		int x;
		int y;
	};

	class AndroidAppState
	{
	private:
		AndroidAppState();
		~AndroidAppState();

		friend void ::ANativeActivity_createFunc(ANativeActivity* activity, void* savedState, size_t savedStateSize);

	public:
		AndroidAppState(const AndroidAppState& other) = delete;
		AndroidAppState(AndroidAppState&& other) = delete;
		AndroidAppState& operator=(const AndroidAppState& other) = delete;
		AndroidAppState& operator=(AndroidAppState&& other) = delete;

		ANativeWindow* GetWindow() const;
		ANativeWindow* GetPendingWindow() const;
		ANativeActivity* GetActivity() const;

	private:
		ANativeWindow* m_CurrentWindow;
		ANativeWindow* m_PendingWindow;
		ANativeActivity* m_Activity;

	public:
		static AndroidAppState& GetInstance();
	};
}
#endif