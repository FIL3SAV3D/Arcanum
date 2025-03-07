#pragma once

#include <ServerClient/SharedInfo.h>
#include <Net/NetServer.h>

namespace arc
{
	class ServerInstance : public arc::net::ServerInterface<ServerClientMsg>
	{
	public:
		ServerInstance(uint16_t port) :
			arc::net::ServerInterface<ServerClientMsg>(port)
		{
			Start();
			SubMainThr = std::thread([this]() { SubMain(); });
		}
		~ServerInstance()
		{
			Stop();
		}

		int SubMain()
		{
			while (1)
			{
				Update(-1);
			}

			return 0;
		}

	private:
		std::thread SubMainThr;


	protected:
		virtual bool OnClientConnect(std::shared_ptr<arc::net::connection<ServerClientMsg>> client)
		{
			arc::net::Message<ServerClientMsg> msg;
			msg.header.id = ServerClientMsg::ServerAccept;
			client->Send(msg);

			return true;
		}

		virtual void OnClientDisconnect(std::shared_ptr<arc::net::connection<ServerClientMsg>> client)
		{
			std::cout << "Removing client [" << client->GetID() << "]\n";
		}

		virtual void OnMessage(std::shared_ptr<arc::net::connection<ServerClientMsg>> client, arc::net::Message<ServerClientMsg>& msg)
		{
			switch (msg.header.id)
			{
			case ServerClientMsg::ServerAccept:
			{
				break;
			}
			case ServerClientMsg::ServerDeny:
				break;
			case ServerClientMsg::ServerPing:
			{
				std::cout << "[" << client->GetID() << "]: Server Ping\n";
				client->Send(msg);
			}
			break;
			case ServerClientMsg::MessageAll:
			{
				std::cout << "[" << client->GetID() << "]: Message All\n";

				// Construct a new message and send it to all clients
				arc::net::Message<ServerClientMsg> msg;
				msg.header.id = ServerClientMsg::MessageAll;
				msg << client->GetID();

				MessageAllClients(msg, client);
			}
			break;
			case ServerClientMsg::ServerMessage:
			{
				break;
			}

			case ServerClientMsg::SpawnEntity:
			{
				std::cout << "[" << client->GetID() << "]: Spawning Entity To All\n";

				MessageAllClients(msg, client);
			}
			break;
			case ServerClientMsg::NewUser:
			{
				std::cout << "[" << client->GetID() << "]: Spawning New User To All\n";
				msg << client->GetID();
				MessageAllClients(msg, client);
			}
			break;
			case ServerClientMsg::UserSync:
			{
				std::cout << "[" << client->GetID() << "]: Syncing User To All\n";
				MessageAllClients(msg, client);
			}
			break;
			default:
				break;
			}
		}
	};
}
