#pragma once

#include "..\Defines.h"
#include "..\Types.h"
#include "..\System\KeyCodes.h"

namespace RayEngine
{
	namespace System
	{
		enum EVENT_TYPE
		{
			EVENT_TYPE_UNKNOWN = 0,
			EVENT_TYPE_QUIT = 1,
			EVENT_TYPE_RESIZE = 2,
			EVENT_TYPE_KEYPRESSED = 3,
			EVENT_TYPE_KEYRELEASED = 4,
			EVENT_TYPE_DESTROYED = 5,
			EVENT_TYPE_APP_PAUSED = 6,
			EVENT_TYPE_APP_RESUMED = 7,
		};

		enum EVENT_RESIZE
		{
			EVENT_RESIZE_UNKNOWN = 0,
			EVENT_RESIZE_NEW_SIZE = 1,
			EVENT_RESIZE_MAXIMIZED = 2,
			EVENT_RESIZE_MINIMIZED = 3,
		};

		struct RE_API Event
		{
		public:
			Event(EVENT_TYPE type = EVENT_TYPE_UNKNOWN, int64 value = 0);

			bool operator==(const Event& other) const;
			bool operator!=(const Event& other) const;

		public:
			EVENT_TYPE Type;
			
			union 
			{
				//Value to set everything to zero
				int64 Value;

				//Struct that contains different parameters
				struct
				{
					union 
					{
						EVENT_RESIZE ResizeType;
						KEY KeyCode;
					};

					union
					{
						int16 Height;
						int16 KeyRepeatCount;
					};

					union
					{
						int16 Width;
						bool KeyExtended;
					};
				};
			};
		};
	}
}