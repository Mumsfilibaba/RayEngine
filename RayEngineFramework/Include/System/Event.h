#pragma once

#include "..\Defines.h"
#include "..\Types.h"
#include "..\System\KeyCodes.h"

namespace RayEngine
{
	namespace System
	{
		enum EVENT_TYPE : int8
		{
			EVENT_TYPE_UNKNOWN = 0,
			EVENT_TYPE_QUIT = 1,
			EVENT_TYPE_RESIZE = 2,
			EVENT_TYPE_KEYPRESSED = 3,
			EVENT_TYPE_KEYRELEASED = 4,
			EVENT_TYPE_DESTROYED = 5,
		};

		enum EVENT_RESIZE : int8
		{
			EVENT_RESIZE_UNKNOWN = 0,
			EVENT_RESIZE_NEW_SIZE = 1,
			EVENT_RESIZE_MAXIMIZED = 2,
			EVENT_RESIZE_MINIMIZED = 3,
		};

		struct Event
		{
		public:
			Event(EVENT_TYPE type = EVENT_TYPE_UNKNOWN, int64 value = 0);

			bool operator==(const Event& other) const;
			bool operator!=(const Event& other) const;

		public:
			EVENT_TYPE type;
			
			union 
			{
				//Value to set everything to zero
				int64 value;

				//Struct that contains different parameters
				struct
				{
					union 
					{
						EVENT_RESIZE resizeType;
						KEY keyCode;
					};

					union
					{
						int16 height;
						int16 keyRepeatCount;
					};

					union
					{
						int16 width;
						bool keyExtended;
					};
				};
			};
		};
	}
}