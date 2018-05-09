#pragma once
#include "..\System\Cursor.h"
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

namespace RayEngine
{
	namespace System
	{
		class Win32CursorImpl : public CursorImpl
		{
		public:
			Win32CursorImpl(const Win32CursorImpl& other) = delete;
			Win32CursorImpl(Win32CursorImpl&& other) = delete;
			Win32CursorImpl& operator=(const Win32CursorImpl& other) = delete;
			Win32CursorImpl& operator=(Win32CursorImpl&& other) = delete;

			Win32CursorImpl();
			~Win32CursorImpl();

			bool LoadFromFile(const Tchar* filename, const Tchar* filepath, int32 x, int32 y) override final;
			bool LoadFromOS(STANDARD_CURSOR Cursor) override final;

			bool IsShared() const;

			CursorImpl* Copy() override final;
			CursorImpl* Move() override final;

			HCURSOR  GetHCURSOR() const;

		private:
			bool m_Shared;
			HCURSOR m_Hcursor;
		};
	}
}