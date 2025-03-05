#include <iostream>

#include <Net/NetCommon.h>

std::vector<char> vBuffer(20 * 1024);

void GrabSomeData(asio::ip::tcp::socket& socket)
{
	socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()), 
		[&](std::error_code ErrorCode, std::size_t length)
		{
			if (!ErrorCode)
			{
				std::cout << "\n\nRead" << length << " bytes\n\n";

				for (int i = 0; i < length; i++)
				{
					std::cout << vBuffer[i];

					GrabSomeData(socket);
				}
			}
		});
}

int main()
{
	/*SOCKET clientSocket{};*/
	try
	{
		asio::error_code ErrorCode;

		asio::io_context Context;

		asio::io_context::work idleWork(Context);

		std::thread ThreadContext = std::thread([&]() { Context.run(); });

		asio::ip::tcp::endpoint Endpoint(asio::ip::make_address("51.38.81.49", ErrorCode), 80);

		asio::ip::tcp::socket Socket(Context);

		Socket.connect(Endpoint, ErrorCode);

		if (!ErrorCode)
		{
			std::cout << "Connected!" << std::endl;
		}
		else
		{
			std::cout << "Failed to connect to adress:\n" << ErrorCode.message() << std::endl;
		}

		if (Socket.is_open())
		{
			GrabSomeData(Socket);

			std::string sRequest =
				"GET /index.html HTTP/1.1\r\n"
				"Host: example.com\r\n"
				"Connection: close\r\n\r\n";

			Socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ErrorCode);

			using namespace std::chrono_literals;
			std::this_thread::sleep_for(20000ms);

			Context.stop();
			if (ThreadContext.joinable())
				ThreadContext.join();
		}

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