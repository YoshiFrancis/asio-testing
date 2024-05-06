#include "server.h"
#include "connection.h" // Assuming this is needed for connection class

void server::do_accept()
{
    m_acceptor.async_accept(
        [this](std::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
							std::cout << "Someone has joined!\n";
							std::make_shared<connection>(std::move(socket), m_room, connection::owner::client)->start();
            }
        });
}
