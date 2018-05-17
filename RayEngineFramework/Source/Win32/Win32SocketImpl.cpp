#include "..\..\Include\Win32\Win32SocketImpl.h"

namespace RayEngine
{
	namespace Network
	{
		Win32SocketIPv4Impl::Win32SocketIPv4Impl()
			: m_Socket(INVALID_SOCKET)
		{
		}

		Win32SocketIPv4Impl::~Win32SocketIPv4Impl()
		{
			Close();
		}

		bool Win32SocketIPv4Impl::Create(SOCK sock)
		{
			int type = 0;
			if (sock == SOCK_STREAM)
				type = SOCK_STREAM;
			else if (sock == SOCK_DGRAM)
				type = SOCK_DGRAM;

			m_Socket = socket(AF_INET, type, );
			return false;
		}


	}
}