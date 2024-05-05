#include "connection.h"

std::vector<uint8_t> testing(128);

connection::connection(tcp::socket socket, game_room& room, owner owner_type)
    : m_socket(std::move(socket)), m_room(room), m_owner(std::move(owner_type))
{
}

void connection::disconnect()
{
    m_socket.close();
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
    async_read(m_socket, asio::buffer(&tmpMsgIn.header, sizeof(tmpMsgIn.header)),
        [this](const std::error_code& ec, size_t len)
        {
            if (!ec)
            {
                readBody();
            }
            else
            {
                disconnect();
            }
        });
}

void connection::readBody()
{
    async_read(m_socket, asio::buffer(tmpMsgIn.body.data(), tmpMsgIn.body.size()),
        [this](const std::error_code& ec, size_t len)
        {
            if (!ec)
            {
                readHeader();
            }
            else
            {
                disconnect();
            }
        });
}
