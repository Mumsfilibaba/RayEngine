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

#include "..\Defines.h"
#include "..\Types.h"
#include "GlImpl.h"

namespace RayEngine
{
	namespace Graphics
	{
#if defined(RE_PLATFORM_WINDOWS)
#define RE_GL_CLASS_NAME RE_T("GLWND")


		/////////////////////////////////////////////////////////////
		int32 GetColorSpace(FORMAT format);
		int32 GetColorBits(FORMAT format);
		int32 GetDepthBits(FORMAT format);
		int32 GetStencilBits(FORMAT format);
		bool SetPixelFormat(GLNativeDevice hDC, FORMAT backBuffer, FORMAT depthStencil);
		HWND CreateDummyWindow();
#endif

		GLNativeContext GetCurrentContext();
		int32 ShaderTypeToGL(SHADER_TYPE type);
	}
}