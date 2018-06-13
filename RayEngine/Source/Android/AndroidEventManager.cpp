#include "AndroidEventManager.h"

#if defined(RE_PLATFORM_ANDROID)

/////////////////////////////////////////////////////////////
int AndroidInputCallback(int fd, int events, void* data)
{
	using namespace RayEngine;

	//Get eventmanager
	AndroidEventManager* eventManager = reinterpret_cast<AndroidEventManager*>(data);
	if (eventManager != nullptr)
	{
		if (events & ALOOPER_EVENT_INPUT)
		{
			eventManager->ProcessQueuedEvent();
		}
	}
	else
	{
		LOGE("'data' in 'int AndroidInputCallback(int fd, int events, void* data)' was nullptr");
	}

	return 1;
}


namespace RayEngine
{
	AndroidEventManager::AndroidEventManager()
		: m_InputQueue(nullptr),
		m_Looper(nullptr),
		m_Events(),
		m_Mutex()
	{
	}

	AndroidEventManager::~AndroidEventManager()
	{
	}

	void AndroidEventManager::CreateLooper()
	{
		m_Looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
		if (m_Looper == nullptr)
		{
			LOGE("'ALooper_prepare' returned nullptr");
		}
	}

	void AndroidEventManager::ProcessQueuedEvent()
	{
		using namespace Math;

		//Lock
		std::lock_guard<std::mutex> lock(m_Mutex);

		//Failsafe
		if (m_InputQueue == nullptr)
			return;

		//Get event and return if no events exists
		AInputEvent* event = nullptr;
		if (AInputQueue_getEvent(m_InputQueue, &event) < 0)
			return;

		//if it is a pre-dispatched event, return
		if (AInputQueue_preDispatchEvent(m_InputQueue, event))
			return;

		//Get event data
		int32 eventType = AInputEvent_getType(event);
		int32 eventSource = AInputEvent_getSource(event);
		int32 pointerCount = AMotionEvent_getPointerCount(event);

		if (eventType == AINPUT_EVENT_TYPE_MOTION)
		{
			//Touchscreen event
			if (eventSource & AINPUT_SOURCE_TOUCHSCREEN)
			{
				//Loop all pointers
				for (int32 i = 0; i < pointerCount; i++)
				{
					//Get id
					int32 id = AMotionEvent_getPointerId(event, i);

					//Get pixel position - Screen
					Vector2 position(AMotionEvent_getX(event, i), AMotionEvent_getY(event, i));

					//Check if we already has this finger
					if (m_Fingers[id % RE_MAX_FINGERS] == position)
					{
						continue;
					}
					else
					{
						m_Fingers[id % RE_MAX_FINGERS] = position;
					}

					//Get pressure
					float pressure = AMotionEvent_getPressure(event, i);
					float size = AMotionEvent_getSize(event, i);

					//Push event
					System::Event event;
					event.Type = System::EVENT_TYPE_TOUCH;
					event.TouchFingerID = id;
					event.TouchPressure = pressure;
					event.TouchSize = size;
					event.TouchX = position.x;
					event.TouchY = position.y;

					m_Events.push(event);
				}
			}
		}

		//Finish event
		AInputQueue_finishEvent(m_InputQueue, event, 1);
	}

	System::Event AndroidEventManager::PopEvent()
	{
		//Lock
		std::lock_guard<std::mutex> lock(m_Mutex);

		//Get first event and pop threadsafe
		System::Event event;
		if (!m_Events.empty())
		{
			event = m_Events.front();
			m_Events.pop();
		}

		return event;
	}

	void AndroidEventManager::PushEvent(const System::Event& event)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		m_Events.push(event);
	}

	void AndroidEventManager::OnInputQueueChanged(AInputQueue* queue, bool terminate)
	{
		if (terminate)
		{
			//Set queue to nullptr
			if (queue == m_InputQueue)
			{
				AInputQueue_detachLooper(m_InputQueue);
				m_InputQueue = nullptr;
			}
		}
		else
		{
			//Set the queue
			m_InputQueue = queue;

			if (m_Looper != nullptr)
			{
				AInputQueue_attachLooper(m_InputQueue, m_Looper, 1, AndroidInputCallback, this);
			}
			else
			{
				LOGE("Looper is nullptr");
			}
		}
	}
}

#endif