#include "..\..\Include\Android\AndroidAppState.h"

#if defined(RE_PLATFORM_ANDROID)
#include <cstdlib>
#include <android/native_window.h>

extern int main();

void ANativeActivity_createFunc(ANativeActivity* activity, void* savedState, size_t savedStateSize)
{
	using namespace RayEngine;

	SavedState* state = nullptr;
	if (savedState != nullptr)
	{
		if (savedStateSize == sizeof(SavedState))
		{
			state = new SavedState();
			memcpy(state, savedState, sizeof(SavedState));
		}
	}
}

namespace RayEngine
{
	AndroidAppState::AndroidAppState()
		: m_CurrentWindow(nullptr),
		m_PendingWindow(nullptr)
	{
	}

	AndroidAppState::~AndroidAppState()
	{
	}

	ANativeWindow* AndroidAppState::GetWindow() const
	{
		return m_CurrentWindow;
	}

	ANativeWindow* AndroidAppState::GetPendingWindow() const
	{
		return m_PendingWindow;
	}

	ANativeActivity* AndroidAppState::GetActivity() const
	{
		return m_Activity;
	}

	AndroidAppState& AndroidAppState::GetInstance()
	{
		static AndroidAppState instance;
		return instance;
	}
}

#endif