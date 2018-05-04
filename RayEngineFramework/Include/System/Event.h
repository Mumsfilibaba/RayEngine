#pragma once

#include "..\Defines.h"
#include "..\Types.h"

#define GET_EVENT_WIDTH(ev) (uint16)(ev.value >> 16)
#define GET_EVENT_HEIGHT(ev) (uint16)((ev.value << 16) >> 16)
#define SET_EVENT_WIDTH(ev, width) ev.value = ((ev.value << 16) >> 16) | (width << 16)
#define SET_EVENT_HEIGHT(ev, height) ev.value = ((ev.value >> 16) << 16) | height
#define GET_EVENT_KEY(ev) (KEY)((ev.value << 1) >> 1)
#define SET_EVENT_KEY(ev, key) ev.value = (uint32)key

namespace RayEngine
{
	namespace System
	{
		enum EVENT_TYPE : uint8
		{
			EVENT_TYPE_UNKNOWN = 0,
			EVENT_TYPE_QUIT = 1,
			EVENT_TYPE_RESIZE = 2,
			EVENT_TYPE_KEYPRESSED = 3,
			EVENT_TYPE_KEYRELEASED = 4,
		};

		struct Event
		{
		public:
			Event();
			Event(EVENT type, uint32 value);

			bool operator==(const Event& other) const;
			bool operator!=(const Event& other) const;

		public:
			EVENT type = EVENT_UNKNOWN;
			uint32 value = 0;
		};
	}
}