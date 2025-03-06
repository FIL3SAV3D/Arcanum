#include <iostream>

#include <ArcNet.h>

enum class CommonMsgs : uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,

	SpawnEntity,
	NewUser,
	UserSync,
	ServerSync,
};

class CustomServer : public arc::net::ServerInterface<CommonMsgs>
{
public:
	CustomServer(uint16_t port) : arc::net::ServerInterface<CommonMsgs>(port)
	{

	}

protected:
	virtual bool OnClientConnect(std::shared_ptr<arc::net::connection<CommonMsgs>> client)
	{
		arc::net::Message<CommonMsgs> msg;
		msg.header.id = CommonMsgs::ServerAccept;
		client->Send(msg);

		return true;
	}

	virtual void OnClientDisconnect(std::shared_ptr<arc::net::connection<CommonMsgs>> client)
	{
		std::cout << "Removing client [" << client->GetID() << "]\n";
	}

	virtual void OnMessage(std::shared_ptr<arc::net::connection<CommonMsgs>> client, arc::net::Message<CommonMsgs>& msg)
	{
		switch (msg.header.id)
		{
		case CommonMsgs::ServerAccept:
		{
			break;
		}
		case CommonMsgs::ServerDeny:
			break;
		case CommonMsgs::ServerPing:
		{
			std::cout << "[" << client->GetID() << "]: Server Ping\n";
			client->Send(msg);
		}
			break;
		case CommonMsgs::MessageAll:
		{
			std::cout << "[" << client->GetID() << "]: Message All\n";

			// Construct a new message and send it to all clients
			arc::net::Message<CommonMsgs> msg;
			msg.header.id = CommonMsgs::MessageAll;
			msg << client->GetID();

			MessageAllClients(msg, client);
		}
			break;
		case CommonMsgs::ServerMessage:
		{
			break;
		}

		case CommonMsgs::SpawnEntity:
		{
			std::cout << "[" << client->GetID() << "]: Spawning Entity To All\n";

			MessageAllClients(msg, client);
		}
		break;
		case CommonMsgs::NewUser:
		{
			std::cout << "[" << client->GetID() << "]: Spawning New User To All\n";
			msg << client->GetID();
			MessageAllClients(msg, client);
		}
		break;
		case CommonMsgs::UserSync:
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

int main()
{
	CustomServer server(60000);
	server.Start();

	arc::net::Message<CommonMsgs> SyncPos;
	SyncPos.header.id = CommonMsgs::ServerSync;
	int index = 0;
	while (1)
	{
		server.Update(-1);


		if (index > 1000)
		{
			server.MessageAllClients(SyncPos);
			index = 0;
		}
		else
		{
			index++;
		}
	}

	return 0;
	
}