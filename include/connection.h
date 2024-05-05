#ifndef CONNECTION_H
#define CONNECTION_H

#define ASIO_STANDALONE

#include "asio.hpp"
#include "chat_message.hpp"
#include "game_room.h"
#include <memory> 
#include <vector> 
#include <deque> 
#include <string>
#include <iostream>

using asio::ip::tcp;
typedef std::deque<message> message_deque;

class game_room;

class connection : public std::enable_shared_from_this<connection>
{
public:
    enum class owner
    {
        server,
        client
    };

public:
    connection(tcp::socket socket, game_room& room, owner owner_type);

    void disconnect();
    void deliver(message msg);
    void start();

private:
    void write();
    void readHeader();
    void readBody();

private:
    message_deque msg_queue_in{};
    message_deque msg_queue_out{};
    message tmpMsgIn;
    game_room m_room;
    tcp::socket m_socket;
    owner m_owner = owner::server;
};

#endif
