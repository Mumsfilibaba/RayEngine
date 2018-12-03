/*////////////////////////////////////////////////////////////

Copyright 2018 Alexander Dahlin

Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in
compliance with the License. You may obtain a copy of
the License at

http ://www.apache.org/licenses/LICENSE-2.0

THIS SOFTWARE IS PROVIDED "AS IS". MEANING NO WARRANTY
OR SUPPORT IS PROVIDED OF ANY KIND.

In event of any damages, direct or indirect that can
be traced back to the use of this software, shall no
contributor be held liable. This includes computer
failure and or malfunction of any kind.

////////////////////////////////////////////////////////////*/

#pragma once
#include "GLDevice.h"

#if defined(RE_PLATFORM_WINDOWS)
#include <Windows.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		HWND CreateDummyWindow();

		void SetStandardPixelformat(HDC hDC);


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class GLDeviceWin32 final : public GLDevice
		{
			RE_IMPLEMENT_INTERFACE(GLDeviceWin32);

		public:
			GLDeviceWin32(const DeviceDesc* pDesc);
			GLDeviceWin32(const DeviceDesc* pDesc, HWND hwnd, int32* pPixelFormatAttribs);
			~GLDeviceWin32();

			inline HDC GetHDC() const
			{
				return m_HDC;
			}

			inline HWND GetHWND() const
			{
				return m_Hwnd;
			}

		private:
			void Create(const DeviceDesc* pDesc);

			void Create(const DeviceDesc* pDesc, HWND hwnd, int32* pPixelFormatAttribs);

			bool QueryWGLExtensions(HDC hDC);

		private:
			HDC m_HDC;
			HWND m_Hwnd;
			GLNativeContext m_Context;

			bool m_IsWindowOwner;
		};
	}
}

#endif