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

#include "Defines.h"
#include "Types.h"
#include "Debug/Debug.h"

#if defined(RE_PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>
#include <wrl\client.h>
#include <comdef.h>

#include <dxgi.h>
#include <dxgi1_6.h>
#include <d3d11.h>
#include <d3d12.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>

#include <gl\GL.h>
#include <gl\GLU.h>
#endif

#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>

#include <crtdbg.h>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include <typeinfo>
#include <locale>
#include <codecvt>
#include <chrono>
#include <limits>
#include <algorithm>
#include <utility>