#pragma once

#include "..\Types.h"
#include "..\Defines.h"

namespace RayEngine
{
	namespace System
	{
		enum STANDARD_CURSOR
		{
			STANDARD_CURSOR_UNKOWN = 0,
			STANDARD_CURSOR_STANDARD = 1,
			STANDARD_CURSOR_CROSS = 2,
			STANDARD_CURSOR_ARROW_WAIT = 3,
			STANDARD_CURSOR_ARROW_HELP = 4,
			STANDARD_CURSOR_CLICK = 5,
			STANDARD_CURSOR_WAIT = 6,
			STANDARD_CURSOR_TEXTMARKER = 7,
			STANDARD_CURSOR_DENIED = 8,
			STANDARD_CURSOR_CLICK_PERSON = 9,
			STANDARD_CURSOR_CLICK_LOCATION = 10,
			STANDARD_CURSOR_MOVE = 11,
			STANDARD_CURSOR_SIZE_NS = 12,
			STANDARD_CURSOR_SIZE_NE_SW = 13,
			STANDARD_CURSOR_SIZE_WE = 14,
			STANDARD_CURSOR_SIZE_NW_SE = 15,
		};

		class RE_API CursorImpl
		{
		public:
			CursorImpl(const CursorImpl& other) = delete;
			CursorImpl(CursorImpl&& other) = delete;

			CursorImpl() {}
			virtual ~CursorImpl() {}

			//TODO: Fix size when loading from file

			//Load a image to use as icon from file - See TextureLoader to see available formats
			//'x' and 'y' are the offsets to were the mouseclick will occur - returns true if succeeded
			virtual bool LoadFromFile(const Tchar* const filename, const Tchar* filePath, int32 x, int32 y) = 0;
			//Load a standard icon from operating system
			virtual bool LoadFromOS(STANDARD_CURSOR cursor) = 0;

			//Returns a new instance as a copy of the implementation - the old object is still valid
			virtual CursorImpl* Copy() = 0;
			//Returns a new instance that contains all the old implementation data - the old object is invalidated
			virtual CursorImpl* Move() = 0;
		};

		class RE_API Cursor
		{
		public:
			Cursor();
			Cursor(const Cursor& other);
			Cursor(Cursor&& other);
			~Cursor();

			//Load a image to use as icon from file - See TextureLoader to see available formats
			//'x' and 'y' are the offsets to were the mouseclick will occur - returns true if succeeded
			bool LoadFromFile(const Tchar* filename, const Tchar* filepath, int32 x, int32 y);
			//Loads a standard cursor from the OS - returns true if succeeded
			bool LoadFromOS(STANDARD_CURSOR cursor);

			const CursorImpl* GetImplementation() const;

			Cursor& operator=(const Cursor& other);
			Cursor& operator=(Cursor&& other);

		private:
			CursorImpl* m_Impl;
		};
	}
}