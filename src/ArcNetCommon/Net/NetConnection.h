#pragma once

#include <Net/NetCommon.h>
#include <Net/NetThreadSafeQueue.h>
#include <Net/NetMessage.h>

namespace arc
{
	namespace net
	{
		template<typename T>
		class Connection : public std::enable_shared_from_this<Connection<T>>
		{
		public:
			Connection()
			{

			}

			virtual ~Connection()
			{

			}
			
		public:
			bool ConnectToServer();
			bool Disconnect();
			bool IsConnected() const;

		public:
			bool Send(const Message<T>& msg);

		};
	}
}