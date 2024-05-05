#ifndef GAMEROOM_H
#define GAMEROOM_H

#define ASIO_STANDALONE

#include "asio.hpp"
#include "chat_message.hpp"
#include <deque>
#include <memory>
#include <list>

class connection;

using asio::ip::tcp;
typedef std::deque<message> message_deque;
typedef std::shared_ptr<connection> connection_ptr;

class game_room
{
public:
    void join(connection_ptr conn);
    void leave(connection_ptr conn);
    void send(connection_ptr conn, const message& message);
    void sendAll(const message& message);

private:
    message_deque msg_queue_in{};
    std::list<connection_ptr> m_connections;
};

#endif
