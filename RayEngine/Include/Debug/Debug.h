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
#include "RayEngine.h"

#if !defined(RE_DISABLE_DBG_LOG)
#define LOG_INFO(x) RayEngine::Debug::Log(LOG_SEVERITY_INFO, x)
#define LOG_ERROR(x) RayEngine::Debug::Log(LOG_SEVERITY_ERROR, x)
#define LOG_WARNING(x) RayEngine::Debug::Log(LOG_SEVERITY_WARNING, x)
#else
#define LOG_INFO(x)
#define LOG_ERROR(x)
#define LOG_WARNING(x)
#endif

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum LOG_SEVERITY : int32
	{
		LOG_SEVERITY_UNKNOWN = 0,
		LOG_SEVERITY_INFO = 1,
		LOG_SEVERITY_ERROR = 2,
		LOG_SEVERITY_WARNING = 3,
	};


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class RAYENGINE_API Debug final
	{
		RE_STATIC_CLASS(Debug);

	public:
		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			Logs a message to the debug log output on the system.

			severity - The severity of the message.

			message - The message as a string.

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		static void Log(LOG_SEVERITY severity, const std::string& message);
	};
}