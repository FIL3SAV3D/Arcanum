#include <iostream>

#include <ArcNet.h>
#include <ServerClient/SharedInfo.h>

class CustomServer : public arc::net::ServerInterface<ServerClientMsg>
{
public:
	CustomServer(uint16_t port) : arc::net::ServerInterface<ServerClientMsg>(port)
	{

	}

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

		}
		break;

		case ServerClientMsg::ServerDeny:
		{

		}
		break;

		case ServerClientMsg::ServerPing:
		{
			std::cout << "[" << client->GetID() << "]: Server Ping\n";
			client->Send(msg);
		}
		break;

		case ServerClientMsg::ServerMessageClient:
			break;
		case ServerClientMsg::ServerMessageAll:
			break;
		case ServerClientMsg::ServerMessageAllExceptSender:
			break;
		default:
			break;
		}


		//switch (msg.header.id)
		//{
		//case ServerClientMsg::ClientUpdate:
		//{
		//	std::cout << "[" << client->GetID() << "]: Update\n";
		//	msg.header.id = ServerClientMsg::UserSync;
		//	MessageAllClients(msg);
		//	break;
		//}
		//case ServerClientMsg::ServerDeny:
		//	break;
		//case ServerClientMsg::ServerPing:
		//{
		//	std::cout << "[" << client->GetID() << "]: Server Ping\n";
		//	client->Send(msg);
		//}
		//break;

		//case ServerClientMsg::UserDisconnect:
		//{
		//	std::cout << "[" << client->GetID() << "]: Disconnected\n";
		//	uint32_t userID = client->GetID();
		//	arc::net::Message<ServerClientMsg> msg;
		//	msg.header.id = ServerClientMsg::RelayUserDisconnect;
		//	msg << userID;
		//	MessageAllClients(msg);
		//}
		//break;

		//case ServerClientMsg::MessageAll:
		//{
		//	//std::cout << "[" << client->GetID() << "]: Message All\n";

		//	// Construct a new message and send it to all clients
		//	arc::net::Message<ServerClientMsg> msg;
		//	msg.header.id = ServerClientMsg::MessageAll;
		//	msg << client->GetID();

		//	MessageAllClients(msg, client);
		//}
		//	break;
		//case ServerClientMsg::ServerMessage:
		//{
		//	break;
		//}

		//case ServerClientMsg::SpawnEntity:
		//{
		//	//std::cout << "[" << client->GetID() << "]: Spawning Entity To All\n";

		//	MessageAllClients(msg, client);
		//}
		//break;
		//case ServerClientMsg::NewUser:
		//{
		//	//std::cout << "[" << client->GetID() << "]: Spawning New User To All\n";
		//	msg << client->GetID();
		//	MessageAllClients(msg, client);
		//}
		//break;
		//case ServerClientMsg::UserSync:
		//{
		//	//std::cout << "[" << client->GetID() << "]: Syncing User To All\n";
		//	msg << client->GetID();
		//	MessageAllClients(msg, client);
		//}
		//break;
		//default:
		//	break;
		//}
	}
};

int main()
{
	CustomServer server(60000);
	server.Start();

	auto current_time = std::chrono::high_resolution_clock::now();

	float time = 0.0f;
	while (1)
	{
		auto new_time = std::chrono::high_resolution_clock::now();
		float frame_time = std::chrono::duration<float, std::chrono::seconds::period>(new_time - current_time).count();
		current_time = new_time;

		time += frame_time;

		if (time >= (1.0f / UpdateFrequency))
		{
			server.Update(-1, false);
			//std::cout << "UPDATE:" << time << std::endl;
			time = 0.0f;
		}
	}

	return 0;
	
}