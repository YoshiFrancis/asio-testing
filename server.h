#ifndef SERVER_H
#define SERVER_H

#define ASIO_STANDALONE

#include "asio.hpp"
#include "game_room.h"
#include "chat_message.hpp"

using asio::ip::tcp;

class server
{
public:
    server(asio::io_context& io_context, tcp::endpoint& endpoint);

private:
    tcp::acceptor m_acceptor;
    game_room m_room;

    void do_accept();
};

#endif
