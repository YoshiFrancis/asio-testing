#ifndef GAMEROOM_H
#define GAMEROOM_H
#define ASIO_STANDALONE

#include "asio.hpp"
#include "chat_message.hpp"
#include "connection.hpp"
#include <deque>
#include <memory>
#include <list>

using asio::ip::tcp;
typedef std::deque<message> message_deque;
typedef std::shared_ptr<connection> connection_ptr;

class game_room
{
public:

	void join(connection_ptr conn)
	{
		m_connections.push_back(conn);
	}

	void leave(connection_ptr conn)
	{
		conn->disconnect();
		msg_queue_in.erase(conn.get());
	}

	void message(connection_ptr conn, std::string& message)
	{
		conn->deliver(message);
	}

	void messageAll(std::string& message)
	{
		for (connection_ptr conn : m_connections)
		{
			conn->deliver(message);
		}
	}

private:
	message_deque msg_queue_in{};
	std::list<connection_ptr> m_connections;
};

#endif
