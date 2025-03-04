#include <WinSock2.h>
#include <iostream>


int main()
{
	SOCKET clientSocket{};
	try
	{
		SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		sockaddr_in serverAdress;
		serverAdress.sin_family = AF_INET;
		serverAdress.sin_port = htons(8080);
		serverAdress.sin_addr.S_un.S_addr = INADDR_ANY;

		bind(serverSocket, (struct sockaddr*)&serverAdress, sizeof(serverAdress));
		listen(serverSocket, 5);

		SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);

		int index = 0;
		while (index < 1000)
		{
			char buffer[1024] = { 0 };
			auto test = recv(clientSocket, buffer, sizeof(buffer), 0);
			std::cout << "Message from client: " << buffer << std::endl;
			Sleep(500);
			++index;
		}
		

		closesocket(serverSocket);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		system("pause");

		return EXIT_FAILURE;
	}

	system("pause");

	return EXIT_SUCCESS;
	
}