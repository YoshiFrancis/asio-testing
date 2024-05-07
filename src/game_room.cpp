#include "game_room.h"
#include "connection.h"
void game_room::join(connection_ptr conn)
{
    m_connections.insert(conn);
}

void game_room::leave(connection_ptr conn)
{
	std::cout << "Someone has left!\n";
	m_connections.erase(conn);
}

void game_room::send(connection_ptr conn, const message& message)
{
	std::cout << "Delivering...\n";
	conn->deliver(message);
}

void game_room::sendAll(const message& message)
{
	std::cout << "Delivering message\n";
	for (connection_ptr conn : m_connections)
	{
			conn->deliver(message);
	}
}
