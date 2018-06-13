#pragma once

#include "..\..\Include\Defines.h"

#if defined(RE_PLATFORM_ANDROID)

#include "..\..\Include\Types.h"
#include "..\..\Include\Android\Android.h"
#include "..\Android\AndroidWindowManager.h"
#include "..\Android\AndroidEventManager.h"

#include <android/configuration.h>

namespace RayEngine
{
	class AndroidAppState
	{
	public:
		AndroidAppState(AndroidAppState&& other) = delete;
		AndroidAppState(const AndroidAppState& other) = delete;
		AndroidAppState& operator=(AndroidAppState&& other) = delete;
		AndroidAppState& operator=(const AndroidAppState& other) = delete;

		AndroidAppState();
		~AndroidAppState();

		void SetActivity(ANativeActivity* activity);
		ANativeActivity* GetActivity() const;

		AConfiguration* GetConfiguration() const;

		bool HasFocus() const;
		void OnFocusChanged(RayEngine::int32 hasFocus);

		AndroidEventManager& GetEvents();
		AndroidWindowManager& GetWindow();

	private:
		ANativeActivity* m_Activity;
		AConfiguration* m_Configuration;

		AndroidWindowManager m_Window;
		AndroidEventManager m_Events;

		bool m_HasFocus;
	};
}

#endif