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

class CustomClient : public arc::net::ClientInterface<CustomMsgTypes>
{
public:
	void PingServer()
	{
		arc::net::Message<CustomMsgTypes> msg; 
		msg.header.id = CustomMsgTypes::ServerPing;

		std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

		std::cout << "Sending Ping\n";

		msg << timeNow;

		std::cout << "Size: " << msg.size() << "\n";

		std::chrono::system_clock::time_point test;

		auto msgtest = msg;

		msgtest >> test;

		std::cout << static_cast<int>(msgtest.header.id) << "\n" << test << "\n";

		Send(msg);
	}

	void MessageAll()
	{
		arc::net::Message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::MessageAll;
		Send(msg);
	}
};

int main()
{
	CustomClient C;
	C.Connect("172.16.201.82", 60000);

	bool key[3] = { false, false, false };
	bool old_key[3] = { false, false, false };

	bool bQuit = false;
	while (!bQuit)
	{

		key[0] = GetAsyncKeyState('1') & 0x8000;
		key[1] = GetAsyncKeyState('2') & 0x8000;
		key[2] = GetAsyncKeyState('3') & 0x8000;

		if (key[0] && !old_key[0]) C.PingServer();
		if (key[1] && !old_key[1]) C.MessageAll();
		if (key[2] && !old_key[2]) bQuit = true;

		for (int i = 0; i < 3; i++) 
			old_key[i] = key[i];

		if (C.IsConnected())
		{
			if (!C.Incoming().empty())
			{
				auto msg = C.Incoming().popfront().msg;

				switch (msg.header.id)
				{
				case CustomMsgTypes::ServerAccept:
				{
					std::cout << "Server Accepted Connection\n";
					break;
				}
				case CustomMsgTypes::ServerDeny:
					break;
				case CustomMsgTypes::ServerPing:
				{
					std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
					std::chrono::system_clock::time_point timeThen;
					msg >> timeThen;
					std::cout << "Server pkg Size: " << msg.size() << "\n";
					std::cout << "Ping " << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
					break;
				}
				case CustomMsgTypes::MessageAll:
				{
					uint32_t clientID;
					msg >> clientID;
					std::cout << "Hello from [" << clientID << "]\n";
					break;
				}
				case CustomMsgTypes::ServerMessage:
					
					break;
				default:
					break;
				}
			}
		}
		else
		{
			std::cout << "Server Down\n";
			bQuit = true;
		}
	}
	
	return 0;
}