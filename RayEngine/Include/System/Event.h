#pragma once

#include "..\Defines.h"
#include "..\Types.h"
#include "..\System\KeyCodes.h"
#include "..\System\TimeStamp.h"
#include "..\System\Sensors.h"
#include "..\Math\Vector2.h"
#include "..\Math\Point.h"

namespace RayEngine
{
	namespace System
	{
		enum EVENT_TYPE : int32
		{
			EVENT_TYPE_UNKNOWN = 0,
			EVENT_TYPE_QUIT = 1,
			EVENT_TYPE_RESIZE = 2,
			EVENT_TYPE_KEYCHAR = 3,
			EVENT_TYPE_KEYPRESSED = 4,
			EVENT_TYPE_KEYRELEASED = 5,
			EVENT_TYPE_CLOSE = 6,
			EVENT_TYPE_FOCUSCHANGED = 7,
			EVENT_TYPE_MOUSEMOVE = 8,
			EVENT_TYPE_MOUSEPRESSED = 9,
			EVENT_TYPE_MOUSERELEASED = 10,
			EVENT_TYPE_MOUSESCROLL = 11,
			EVENT_TYPE_TOUCHMOVE = 12,
			EVENT_TYPE_TOUCHPRESSED = 13,
			EVENT_TYPE_TOUCHRELEASED = 14,
			EVENT_TYPE_SENSORCHANGED = 15,
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
	
			union
			{
				struct
				{
					int32 ExitCode;
				} Quit;

				struct
				{
					int32 Width;
					int32 Height;
					EVENT_RESIZE Type;
				} Resize;

				struct
				{
					uint32 UnicodeChar;
				} KeyChar;

				struct
				{
					KEY KeyCode;
					int32 RepeatCount;
					bool Extended;
				} Key;

				struct
				{
					bool HasFocus;
				} FocusChanged;

				struct
				{
					Math::Point Position;
				} MouseMove;

				struct
				{
					MOUSEBUTTON Button;
				} MouseButton;

				struct
				{
					float Delta;
				} MouseScroll;

				struct
				{
					Math::Vector2 Position;
					float Size;
					float Pressure;
					int32 FingerID;
					TimeStamp Time;
				} Touch;

				struct
				{
					SENSOR_TYPE Type;
					Math::Vector3 Data;
				} Sensor;
			};
		};
	}
}
