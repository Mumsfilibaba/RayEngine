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
#include "../Defines.h"
#include "../Types.h"

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class IWindow;
	
	struct WindowDesc;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum PLATFORM
	{
		PLATFORM_UNKNOWN = 0,
		PLATFORM_ANDROID = 1,
		PLATFORM_WIN32 = 2,
		PLATFORM_LINUX = 3
	};


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct SystemDesc
	{
		PLATFORM Platform;
		int32 ScreenWidth;
		int32 ScreenHeight;
	};


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int32 InitializeSystem(int32 systemFlags);
	
	void ReleaseSystem();
	
	void QuerySystemDesc(SystemDesc* pDesc);

	void SendQuitMessage(int32 exitCode);

	IWindow* CreateWindow(WindowDesc* pDesc);
}