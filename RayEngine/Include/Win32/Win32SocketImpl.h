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
#include "..\..\Include\Defines.h"
#if defined (RE_PLATFORM_WINDOWS)

#include "..\Network\Socket.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif

#include <WinSock2.h>

namespace RayEngine
{
	namespace Network
	{
		class RE_API Win32SocketIPv4Impl : public ISocketIPv4Impl
		{
		public:
			Win32SocketIPv4Impl(Win32SocketIPv4Impl&& other) = delete;
			Win32SocketIPv4Impl(const Win32SocketIPv4Impl& other) = delete;
			Win32SocketIPv4Impl& operator=(Win32SocketIPv4Impl&& other) = delete;
			Win32SocketIPv4Impl& operator=(const Win32SocketIPv4Impl& other) = delete;

			Win32SocketIPv4Impl();
			~Win32SocketIPv4Impl();

			bool Create(SOCK sock) override final;
			void Close() const override final;
			
			bool Bind(const Tchar* ip, int32 port) const override final;
			bool Connect(const Tchar* ip, int32 port) const override final;
			bool Listen() const override final;
			ISocketIPv4Impl* Accept() const override final;
			
			bool Send(const void* data, int32 bytes) const override final;
			bool SendTo(const void* data, int32 bytes) const override final;
			bool SendTo(const Tchar* ip, int32 port, const void* data, int32 bytes) const override final;
			
			bool Recv(void* data, int32 bytes) const override final;
			bool RecvFrom() const override final;
			
			bool IsValid() const override final;

		private:
			SOCKET m_Socket;
		};
	}
}
#endif