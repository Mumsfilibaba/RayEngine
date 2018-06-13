#pragma once

#include "..\..\Include\Defines.h"

#if defined(RE_PLATFORM_ANDROID)

#include "..\..\Include\Types.h"
#include "..\..\Include\Math\Vector2.h"
#include "..\..\Include\Android\Android.h"

#include <android\input.h>
#include <android\looper.h>

#include <mutex>
#include <queue>

#define RE_MAX_FINGERS 10

namespace RayEngine
{
	class AndroidEventManager
	{
	public:
		AndroidEventManager(AndroidEventManager&& other) = delete;
		AndroidEventManager(const AndroidEventManager& other) = delete;
		AndroidEventManager& operator=(AndroidEventManager&& other) = delete;
		AndroidEventManager& operator=(const AndroidEventManager& other) = delete;

		AndroidEventManager();
		~AndroidEventManager();

		void CreateLooper();

		void ProcessQueuedEvent();

		System::Event PopEvent();
		void PushEvent(const System::Event& pEvent);

		void OnInputQueueChanged(AInputQueue* queue, bool terminate);

	private:
		std::mutex m_Mutex;
		std::queue<System::Event> m_Events;

		AInputQueue* m_InputQueue;
		ALooper* m_Looper;

		Math::Vector2 m_Fingers[RE_MAX_FINGERS];
	};
}

#endif