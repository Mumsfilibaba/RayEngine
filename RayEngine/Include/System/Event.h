#pragma once

#include "..\Defines.h"
#include "..\Types.h"
#include "..\System\KeyCodes.h"
#include "..\Math\Vector2.h"

namespace RayEngine
{
	namespace System
	{
		enum EVENT_TYPE : int32
		{
			EVENT_TYPE_UNKNOWN = 0,
			EVENT_TYPE_QUIT = 1,
			EVENT_TYPE_RESIZE = 2,
			EVENT_TYPE_KEYPRESSED = 3,
			EVENT_TYPE_KEYRELEASED = 4,
			EVENT_TYPE_DESTROYED = 5,
			EVENT_TYPE_APP_PAUSED = 6,
			EVENT_TYPE_APP_RESUMED = 7,
			EVENT_TYPE_TOUCH = 8,
		};

		enum EVENT_RESIZE : int32
		{
			EVENT_RESIZE_UNKNOWN = 0,
			EVENT_RESIZE_NEW_SIZE = 1,
			EVENT_RESIZE_MAXIMIZED = 2,
			EVENT_RESIZE_MINIMIZED = 3,
		};

		struct RE_API Event
		{
		public:
			Event();
			Event(const Event& other);
			
			Event& operator=(const Event& other);

			bool operator==(const Event& other) const;
			bool operator!=(const Event& other) const;

		public:
			EVENT_TYPE Type;
	
			struct
			{
				Math::Vector2 TouchPosition;

				union
				{
					float TouchSize;
					EVENT_RESIZE ResizeType;
					KEY KeyCode;
				};

				union
				{
					int32 QuitCode;
					int32 TouchFingerID;
					int16 Height;
					int16 KeyRepeatCount;
				};

				union
				{
					float TouchPressure;
					int16 Width;
					bool KeyExtended;
				};
			};
		};
	}
}
