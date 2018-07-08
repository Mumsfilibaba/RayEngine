#pragma once

#include "..\Defines.h"
#include "..\Types.h"

namespace RayEngine
{
	namespace Network
	{
		enum SOCK
		{
			SOCK_UNKNOWN = 0,
			SOCK_DGRAM = 1,
			SOCK_STREAM = 2,
		};

		//Implements an OS-Specific IPv4-Socket
		class RE_API ISocketIPv4Impl
		{
		public:
			ISocketIPv4Impl(ISocketIPv4Impl&& other) = delete;
			ISocketIPv4Impl(const ISocketIPv4Impl& other) = delete;
			ISocketIPv4Impl& operator=(ISocketIPv4Impl&& other) = delete;
			ISocketIPv4Impl& operator=(const ISocketIPv4Impl& other) = delete;
			
			ISocketIPv4Impl() {}
			virtual ~ISocketIPv4Impl() {}

			//Create needed OS-Handles
			virtual bool Create(SOCK sock) = 0;
			//Binds socket to selected IP and port
			virtual bool Bind(const Tchar* ip, int32 port) const = 0;
			//Listen after connections
			virtual bool Listen() const = 0;
			//Accepts a connection and returns the clientsocket
			virtual ISocketIPv4Impl* Accept() const = 0;
			//Connect to another socket
			virtual bool Connect(const Tchar* ip, int32 port) const = 0;
			//Send data 
			virtual bool Send(const void* data, int32 bytes) const = 0;
			//Send to IP and port
			virtual bool SendTo(const void* data, int32 bytes) const = 0;
			virtual bool SendTo(const Tchar* ip, int32 port, const void* data, int32 bytes) const = 0;
			//Recive
			virtual bool Recv(void* data, int32 bytes) const = 0;
			//Recive data at bound port, from is the adress from where the datagram got sent
			virtual bool RecvFrom() const = 0;
			//Check if the socket is valid
			virtual bool IsValid() const = 0;			
			//Closes the socket - The socket will be invalid after call
			virtual void Close() const = 0;
		};

		//Wrapper for SocketIPv4Impl
		class RE_API SocketIPv4
		{
		private:
			//Private default constructor
			SocketIPv4();

		public:
			SocketIPv4(const SocketIPv4& other) = delete;
			SocketIPv4& operator=(const SocketIPv4& other) = delete;

			SocketIPv4(SOCK sock);
			SocketIPv4(SocketIPv4&& other);
			~SocketIPv4();
			
			//Binds socket to the local IP on the selected port
			bool Bind(int32 port) const;
			//Binds socket to selected IP and port
			bool Bind(const Tchar* ip, int32 port) const;
			//Listen after connections
			bool Listen() const;
			//Accepts a connection and returns the clientsocket
			SocketIPv4 Accept() const;
			//Connect to another socket
			bool Connect(const Tchar* ip, int32 port) const;
			//Send data 
			bool Send(const void* data, int32 bytes) const;
			//Send to IP and port
			bool SendTo(const Tchar* ip, int32 port, const void* data, int32 bytes) const;
			//Recive
			bool Recv(void* data, int32 bytes) const;
			//Recive data at bound port, from is the adress from where the datagram got sent
			bool RecvFrom() const;
			//Check if the socket is valid
			bool IsValid() const;
			//Closes the socket - The socket will be invalid after call
			void Close() const;

			SocketIPv4& operator=(SocketIPv4&& other);

			const ISocketIPv4Impl* GetImplementation() const;

		private:
			ISocketIPv4Impl* m_Impl;
		};
	}
}