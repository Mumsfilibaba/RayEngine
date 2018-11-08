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
#include "ILog.h"

namespace RayEngine
{
	class LogService final
	{
		RE_STATIC_CLASS(LogService);

	public:
		static void DebugLog(ILog* pLog);
		
		static ILog* DebugLog();
		
		static void GraphicsLog(ILog* pLog);
		
		static ILog* GraphicsLog();

	private:
		static ILog* s_pGraphicsLog;
		
		static ILog* s_pDebugLog;
	};
}