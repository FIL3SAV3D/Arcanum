#include "TempTest.h"
#include <WinSock2.h>

int main()
{
	printf("WELCOME TO ARCANUM USER\n\n");

	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	auto test = connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	printf("%i", test);
	int index = 0;
	while (index < 1000)
	{
		const char* message = "Hello, server!";
		send(clientSocket, message, strlen(message), 0);
	}

	closesocket(clientSocket);


	arc::cFirstApp app{};

	try
	{
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}