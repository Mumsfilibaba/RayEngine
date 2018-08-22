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

#include "..\..\Include\Win32\Win32SocketImpl.h"

#if defined (RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Network
	{
		/////////////////////////////////////////////////////////////
		Win32SocketIPv4Impl::Win32SocketIPv4Impl()
			: m_Socket(INVALID_SOCKET)
		{
		}



		/////////////////////////////////////////////////////////////
		Win32SocketIPv4Impl::~Win32SocketIPv4Impl()
		{
			Close();
		}



		/////////////////////////////////////////////////////////////
		bool Win32SocketIPv4Impl::Create(SOCK sock)
		{
			int type = 0;
			if (sock == SOCK_STREAM)
				type = SOCK_STREAM;
			else if (sock == SOCK_DGRAM)
				type = SOCK_DGRAM;

			//m_Socket = socket(AF_INET, type, );
			return false;
		}
	}
}

#endif