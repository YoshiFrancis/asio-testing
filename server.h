#ifndef SERVER_H
#define SERVER_H
#define ASIO_STANDALONE
#include "asio.hpp"
#include "game_room.hpp"
#include "chat_message.hpp"

using asio::ip::tcp;

class server 
{
public:
	server(asio::io_context& io_context, tcp::endpoint& endpoints)
		: m_acceptor(io_context, endpoints)
		{
			do_accept();
		}

private:
	tcp::acceptor m_acceptor;
	game_room m_room;
	
	void do_accept()
	{
		m_acceptor.async_accept(
			[this](std::error_code ec, tcp::socket socket)
			{
				if (!ec)
				{
					std::make_shared<connection>(std::move(socket), m_room, message::owner::client)->start();
				}
			});
	}
};

#endif
