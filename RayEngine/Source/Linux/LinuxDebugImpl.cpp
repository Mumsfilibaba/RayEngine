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

#include "../../Include/Debug/Debug.h"
#include "../../Include/System/Clock.h"
#include <iostream>

#if defined(RE_PLATFORM_LINUX)

namespace RayEngine
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Debug::Log(LOG_SEVERITY severity, const std::string& message)
    {
        std::string msg = '[' + Clock::DateString() + ']';
        if (severity == LOG_SEVERITY_INFO)
        {
            std::cout << "\033[1;32m";
            msg += std::string(" [INFO] : ");
        }
        else if (severity == LOG_SEVERITY_WARNING)
        {
            std::cout << "\033[1;33m";
            msg += std::string(" [WARNING] : ");
        }
        else if (severity == LOG_SEVERITY_ERROR)
        {
            std::cout << "\033[1;31m";
            msg += std::string(" [ERROR] : ");  
        }

        std::cout << msg << message << "\033[0m" << std::endl;
    }
}

#endif