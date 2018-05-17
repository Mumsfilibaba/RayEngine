#include "..\..\Include\Network\Socket.h"
#include <cassert>

namespace RayEngine
{
	namespace Network
	{
		SocketIPv4::SocketIPv4()
			: m_Impl(nullptr)
		{
		}

		SocketIPv4::SocketIPv4(SOCK sock)
			: m_Impl(nullptr)
		{
			//TODO: Create implementation
		}

		SocketIPv4::SocketIPv4(SocketIPv4&& other)
			: m_Impl(other.m_Impl)
		{
			other.m_Impl = nullptr;
		}
		
		SocketIPv4::~SocketIPv4()
		{
			if (m_Impl != nullptr)
			{
				delete m_Impl;
				m_Impl = nullptr;
			}
		}
		
		bool SocketIPv4::Bind(int32 port) const
		{
			assert(IsValid());
			return m_Impl->Bind(nullptr, port);
		}

		bool SocketIPv4::Bind(const Tchar* ip, int32 port) const
		{
			assert(IsValid());
			return m_Impl->Bind(ip, port);
		}

		bool SocketIPv4::Listen() const
		{
			assert(IsValid());
			return m_Impl->Listen();
		}
		
		SocketIPv4 SocketIPv4::Accept() const
		{
			assert(IsValid());

			SocketIPv4 result;
			result.m_Impl = m_Impl->Accept();
			return result;
		}
		bool SocketIPv4::Connect(const Tchar* ip, int32 port) const 
		{
			assert(IsValid());
			return m_Impl->Connect(ip, port);
		}

		bool SocketIPv4::Send(const void* data, int32 bytes) const
		{
			assert(IsValid());
			return m_Impl->Send(data, bytes);
		}

		bool SocketIPv4::SendTo(const Tchar* ip, int32 port, const void* data, int32 bytes) const
		{
			assert(IsValid());
			return m_Impl->SendTo(ip, port, data, bytes);
		}
		
		bool SocketIPv4::Recv(void* data, int32 bytes) const
		{
			assert(IsValid());
			return m_Impl->Recv(data, bytes);
		}

		bool SocketIPv4::RecvFrom() const
		{
		}

		bool SocketIPv4::IsValid() const
		{
			return (m_Impl == nullptr) ? false : m_Impl->IsValid();
		}

		void SocketIPv4::Close() const
		{
			assert(IsValid());
			m_Impl->Close();
		}

		SocketIPv4& SocketIPv4::operator=(SocketIPv4&& other)
		{
			if (this != &other)
			{
				m_Impl = other.m_Impl;
				other.m_Impl = nullptr;
			}

			return *this;
		}

		const SocketIPv4Impl* SocketIPv4::GetImplementation() const
		{
			return m_Impl;
		}
	}
}