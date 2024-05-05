#include "game_room.h"

void game_room::join(connection_ptr conn)
{
    m_connections.push_back(conn);
}

void game_room::leave(connection_ptr conn)
{
    conn->disconnect();
    msg_queue_in.erase(conn.get());
}

void game_room::send(connection_ptr conn, const message& message)
{
    conn->deliver(message);
}

void game_room::sendAll(const message& message)
{
    for (connection_ptr conn : m_connections)
    {
        conn->deliver(message);
    }
}
