#pragma once

#include <Net/NetCommon.h>
#include <Net/NetThreadSafeQueue.h>
#include <Net/NetMessage.h>
#include <Net/NetConnection.h>

namespace arc
{
	namespace net
	{
		template<typename T>
		class ServerInterface
		{
		public:
			ServerInterface(uint16_t port):
				MAsioAcceptor(MAsioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
			{

			}

			virtual ~ServerInterface()
			{
				Stop();
			}

			bool Start()
			{
				try
				{
					WaitForClientConnection();
					ThreadContext = std::thread([this]() { MAsioContext.run(); });
				}
				catch (std::exception& e)
				{
					std::cerr << "[ARCSPHERE] Exception: " << e.what() << "\n";
					return false;
				}

				std::cout << "[ARCSPHERE] Started!\n";
				return true;
			}

			void Stop()
			{
				MAsioContext.stop();

				if (ThreadContext.joinable())
					ThreadContext.join();

				std::cout << "[ARCSPHERE] Stopped!\n";
			}

			//ASYNC
			void WaitForClientConnection()
			{
				MAsioAcceptor.async_accept(
					[this](std::error_code ec, asio::ip::tcp::socket socket)
					{
						if (!ec)
						{
							std::cout << "[ARCSPHERE] New Wizard: " << socket.remote_endpoint() << "\n";

							std::shared_ptr<connection<T>> newconn =
								std::make_shared<connection<T>>(connection<T>::owner::server,
									MAsioContext, std::move(socket), MQMessagesIn);

							if (OnClientConnect(newconn))
							{
								MDeqConnections.push_back(std::move(newconn));

								MDeqConnections.back()->ConnectToClient(this, nIDCounter++);

								std::cout << "[" << MDeqConnections.back()->GetID() << "] Wizard Approved\n";
							}
							else
							{
								std::cout << "[-----] Connection Denied\n";
							}
						}
						else
						{
							std::cout << "[ARCSPHERE] New Wizard Error: " << ec.message() << "\n";
						}

						WaitForClientConnection();
					});
			}

			void MessageClient(std::shared_ptr<connection<T>> client, const Message<T>& msg)
			{
				if (client && client->IsConnected())
				{
					client->Send(msg);
				}
				else
				{
					OnClientDisconnect(client);
					client.reset();
					MDeqConnections.erase(
						std::remove(MDeqConnections.begin(), MDeqConnections.end(), client), MDeqConnections.end());
				}
			}

			void MessageAllClients(const Message<T>& msg, std::shared_ptr<connection<T>> ignoreClient = nullptr )
			{
				bool InvalidClientExists = false;

				for (auto& client : MDeqConnections)
				{
					if (client && client->IsConnected())
					{
						if (client != ignoreClient)
						{
							client->Send(msg);
						}
					}
					else
					{
						OnClientDisconnect(client);
						client.reset();
						InvalidClientExists = true;
					}
				}

				if(InvalidClientExists)
					MDeqConnections.erase(
						std::remove(MDeqConnections.begin(), MDeqConnections.end(), nullptr), MDeqConnections.end());
			}

			void Update(size_t nMaxMessages = -1, bool wait = false)
			{
				if (wait)
					MQMessagesIn.wait();

				size_t MessageCount = 0;

				while (MessageCount < nMaxMessages && !MQMessagesIn.empty())
				{
					OwnedMessage<T> msg = MQMessagesIn.popfront();

					OnMessage(msg.remote, msg.msg);

					MessageCount++;
				}
			}

		protected:
			virtual bool OnClientConnect(std::shared_ptr<connection<T>> client)
			{
				return false;
			}

			virtual void OnClientDisconnect(std::shared_ptr<connection<T>> client)
			{

			}

			virtual void OnMessage(std::shared_ptr<connection<T>> client, Message<T>& msg)
			{

			}

		public:
			virtual void OnClientValidated(std::shared_ptr<connection<T>> client)
			{

			}

		protected:
			TSQueue<OwnedMessage<T>> MQMessagesIn;

			std::deque<std::shared_ptr<connection<T>>> MDeqConnections;

			asio::io_context MAsioContext;

			std::thread ThreadContext;

			asio::ip::tcp::acceptor MAsioAcceptor;

			uint32_t nIDCounter = 10000;
		};
	}
}