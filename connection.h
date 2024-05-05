#ifndef CONNECTION_H
#define CONNECTION_H
#define ASIO_STANDALONE
#include "asio.hpp"
#include "chat_message.hpp"
#include "game_room.hpp"
#include <memory> 			// used for the shared_ptr to hold instances of chat participants
#include <vector> // hold buffer for incomming messages
#include <deque> // deque for the messages
#include <string>
#include <iostream>

using asio::ip::tcp;
typedef std::deque<message> message_deque;


class connection : public std::enable_shared_from_this<connection>
{
public:
	enum class owner
	{
		server,
		client
	};

public:

	connection(tcp::socket socket, game_room& room, owner owner_type)
		: m_socket(std::move(socket), m_room { room }, m_owner(std::move(owner_type)))
	{
	}

	void connect(tcp::resolver::results_type& endpoints)
	{
		async_connect(m_socket, endpoints, connect_handler);
	}

	void disconnect()
	{
		m_socket.close();
		m_room.leave(std::shared_from_this());
	}

	void deliver(message&& msg)
	{
		msg_queue_out.push_back(std::move(msg));
		if (msg_queue_out.empty())
		{
			write();
		}
	}

	void start()
	{
		readHeader();
	}

private:

	void handle_connect(std::error_code& ec, tcp::endpoint endpoint)
	{
		if (!ec)
		{
			// begin to read
			readHeader();
		}
	}

	void write()
	{
		async_write(m_socket, asio::buffer(msg_queue_out.front().data(), msg_queue_out.front().size()), 
			[this](std::error_code& ec, size_t len)
			{
				if (!ec)
				{
					msg_queue_out.pop_front();
					write();
				}
				else
				{
					disconnect();
				}
			});
	}
			
	void readHeader()
	{
		async_read(m_socket, asio::buffer(tmpMsgIn.header.body.data()), 
			[this](std::error_code& ec, size_t len)
			{
				if (!ec)
				{
					readBody();
				}
				else
				{
					disconnect();
				}
			});
	}

	void readBody()
	{
		async_read(m_socket, asio::buffer(tmpMsgIn.body.data(), tmpMsgIn.body.size()), 
			[this](std::error_code& ec, size_t len)
			{
				if (!ec)
				{
					readHeader();
					std::cout << tmpMsgIn.body();
				}
				else
				{
					disconnect();
				}
			});
	}

private:
	message_deque msg_queue_in{};
	message_deque msg_queue_out{};
	message tmpMsgIn;
	game_room m_room;
	tcp::socket m_socket;
	owner m_owner = owner::server;
	asio::io_context& m_io_context;
};

#endif
