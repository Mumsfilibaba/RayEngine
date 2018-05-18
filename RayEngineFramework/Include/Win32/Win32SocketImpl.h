#pragma once

#include "..\..\Include\Defines.h"
#if defined (RE_PLATFORM_WINDOWS)

#include "..\Network\Socket.h"
#ifndef WIN32_LEAN_AND_MEAN 1
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <WinSock2.h>

namespace RayEngine
{
	namespace Network
	{
		class Win32SocketIPv4Impl : public SocketIPv4Impl
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
			SocketIPv4Impl* Accept() const override final;
			
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