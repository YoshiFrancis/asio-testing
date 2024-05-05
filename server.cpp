#include "server.h"
#include "connection.h" // Assuming this is needed for connection class

server::server(asio::io_context& io_context, tcp::endpoint& endpoint)
    : m_acceptor(io_context, endpoint)
{
    do_accept();
}

void server::do_accept()
{
    m_acceptor.async_accept(
        [this](std::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                std::make_shared<connection>(std::move(socket), m_room, connection::owner::client)->start();
            }
        });
}
