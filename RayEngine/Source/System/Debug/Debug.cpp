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

#include "..\..\..\Include\System\Debug\Debug.h"
#include "..\..\..\Include\System\Log\OutputLog.h"

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Debug::EnableDebugLog()
	{
		if (LogService::DebugLog() == nullptr)
			LogService::DebugLog(new OutputLog);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Debug::DisableDebugLog()
	{
		LogService::DebugLog(nullptr);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Debug::Log(LOG_SEVERITY severity, const std::string& message)
	{
		ILog* pLog = LogService::DebugLog();
		if (pLog != nullptr)
			pLog->Write(severity, message);
	}
}