#include "server.h"
#include "asio.hpp"
#include <iostream>

using asio::ip::tcp;

int main(int argc, char* argv[])
{
	
	try
	{
		if (argc < 2)
		{
			std::cout << "Usage: <port>\n";
			return 1;
		}

		asio::io_context io_context;
		tcp::resolver resolver(io_context);
		tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[1]));
		server my_server { io_context, endpoint };
		io_context.run();
	
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << "\n";
		return 0;
	}


	return 1;
}

