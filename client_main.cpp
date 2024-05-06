#define ASIO_STANDALONE
#include "asio.hpp"
#include "client.h"
#include "chat_message.hpp"

using asio::ip::tcp;

int main(int argc, char* argv[])
{

	std::cout << "I attempt\n";
	try
	{
		if (argc != 3)
		{
			std::cout << "Usage <host> <port>\n";
			return 0;
		}

		asio::io_context io_context;
		tcp::resolver resolver(io_context);
		auto endpoints = resolver.resolve(argv[1], argv[2]);
		client c(io_context, endpoints);
		std::thread t([&io_context]() { io_context.run(); });
		io_context.run();

	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << "\n";
		return 0;
	}
	
	return 1;
}
