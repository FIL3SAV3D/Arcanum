#include <iostream>

#include <ArcNet.h>

enum class CustomMsgTypes : uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,
};

class CustomServer : public arc::net::ServerInterface<CustomMsgTypes>
{
public:
	CustomServer(uint16_t port) : arc::net::ServerInterface<CustomMsgTypes>(port)
	{

	}

protected:
	virtual bool OnClientConnect(std::shared_ptr<arc::net::connection<CustomMsgTypes>> client)
	{
		arc::net::Message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerAccept;
		client->Send(msg);

		return true;
	}

	virtual void OnClientDisconnect(std::shared_ptr<arc::net::connection<CustomMsgTypes>> client)
	{
		std::cout << "Removing client [" << client->GetID() << "]\n";
	}

	virtual void OnMessage(std::shared_ptr<arc::net::connection<CustomMsgTypes>> client, arc::net::Message<CustomMsgTypes>& msg)
	{
		switch (msg.header.id)
		{
		case CustomMsgTypes::ServerAccept:
		{
			break;
		}
		case CustomMsgTypes::ServerDeny:
			break;
		case CustomMsgTypes::ServerPing:
		{
			std::cout << "[" << client->GetID() << "]: Server Ping\n";
			client->Send(msg);
		}
			break;
		case CustomMsgTypes::MessageAll:
		{
			std::cout << "[" << client->GetID() << "]: Message All\n";

			// Construct a new message and send it to all clients
			arc::net::Message<CustomMsgTypes> msg;
			msg.header.id = CustomMsgTypes::MessageAll;
			msg << client->GetID();

			MessageAllClients(msg, client);
		}
			break;
		case CustomMsgTypes::ServerMessage:
		{
			break;
		}
		default:
			break;
		}
	}
};

int main()
{
	CustomServer server(60000);
	server.Start();

	while (1)
	{
		server.Update(-1);
	}

	return 0;
	
}