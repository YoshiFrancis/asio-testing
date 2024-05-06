#include "connection.h"

std::vector<uint8_t> testing(128);

void connection::disconnect()
{
    m_room.leave(shared_from_this());
}

void connection::deliver(message msg)
{
    msg_queue_out.push_back(std::move(msg));
    if (msg_queue_out.empty())
    {
        write();
    }
}

void connection::start()
{
	readHeader();
}

void connection::write()
{
    async_write(connection::m_socket, asio::buffer(testing),
        [this](const std::error_code& ec, size_t len)
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

void connection::readHeader()
{
    auto self(shared_from_this());
    async_read(m_socket, asio::buffer(testing, testing.size()),
        [this, self](const std::error_code ec, size_t len)
        {
            if (!ec)
            {
							std::cout << "Attempting to read here!!\n";
							readBody();
            }
            else
						{
							std::cout << "Reading Header: " << ec.message() << "\n";
							if (m_owner == owner::client)
							{
								disconnect();
							}
            }
        });
}

void connection::readBody()
{
    auto self(shared_from_this());
    async_read(m_socket, asio::buffer(tmpMsgIn.body.data(), tmpMsgIn.body.size()),
        [this](const std::error_code& ec, size_t len)
        {
            if (!ec)
            {
								m_room.sendAll(tmpMsgIn);
                readHeader();
            }
            else
            {
                disconnect();
            }
        });
}
