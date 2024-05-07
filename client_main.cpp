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
		
		std::string input{};
		std::getline(std::cin, input);
		/*
		while (std::getline(std::cin, input))
		{
			message msg{};
			msg << input;
			c.deliver(msg);
		}
		*/
		message msg{};
		msg << input;
		c.deliver(msg);
		c.disconnect();
		t.join();
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << "\n";
		return 0;
	}
	
	return 1;
}
