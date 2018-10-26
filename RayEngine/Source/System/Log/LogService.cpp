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

#include "..\..\..\Include\System\Log\LogService.h"

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ILog* LogService::s_pDebugLog = nullptr;
	ILog* LogService::s_pGraphicsLog = nullptr;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void LogService::DebugLog(ILog* pLog)
	{
		if (s_pDebugLog != nullptr)
			delete s_pDebugLog;

		s_pDebugLog = pLog;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ILog* LogService::DebugLog()
	{
		return s_pDebugLog;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void LogService::GraphicsLog(ILog* pLog)
	{
		if (s_pGraphicsLog != nullptr)
			delete s_pGraphicsLog;

		s_pGraphicsLog = pLog;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ILog* LogService::GraphicsLog()
	{
		return s_pGraphicsLog;
	}
}