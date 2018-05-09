#pragma once

#include "..\System\Icon.h"
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

namespace RayEngine
{
	namespace System
	{
		class Win32IconImpl : public IconImpl
		{
		public:
			Win32IconImpl(const Win32IconImpl& other) = delete;
			Win32IconImpl(Win32IconImpl&& other) = delete;
			Win32IconImpl& operator=(const Win32IconImpl& other) = delete;
			Win32IconImpl& operator=(Win32IconImpl&& other) = delete;

			Win32IconImpl();
			~Win32IconImpl();

			bool LoadFromFile(const Tchar* filename, const Tchar* filepath) override final;
			bool LoadFromOS(STANDARD_ICON icon) override final;

			IconImpl* Copy() override final;
			IconImpl* Move() override final;

			HICON GetHICON() const;

		private:
			HICON m_Hicon;
		};
	}
}