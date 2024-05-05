#ifndef CLIENT_H
#define CLIENT_H

#define ASIO_STANDALONE

#include "asio.hpp"
#include "chat_message.hpp"
#include "connection.h"
#include <memory>
#include <deque>
#include <iostream>

using asio::ip::tcp;
typedef std::shared_ptr<connection> connection_ptr;
typedef std::deque<message> message_queue;

class client
{
public:
    client(asio::io_context& io_context, tcp::endpoint& endpoint);

    void disconnect();
    void deliver(message message);

private:
    void readHeader();
    void readBody();
    void write();

private:
    message_queue m_msg_queue_in{};
    message_queue m_msg_queue_out{};
    message m_tmp_msg_in;
    asio::io_context& m_io_context;
    tcp::socket m_socket;
};

#endif
