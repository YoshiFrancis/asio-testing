#include "connection.h"

static std::vector<uint8_t> _serialize(const message& msg);
static message_header _deserialize_header(const std::vector<uint8_t>& buffer);
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
		
    async_write(connection::m_socket, asio::buffer(_serialize(msg_queue_out.front()).data(), sizeof(msg_queue_out.front())),
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
		tmpMsgHeaderBuffer.resize(sizeof(message_header));
    async_read(m_socket, asio::buffer(tmpMsgHeaderBuffer.data(), sizeof(message_header)),
        [this, self](const std::error_code ec, size_t len)
        {
            if (!ec)
            {
							tmpMsgIn.header = _deserialize_header(tmpMsgHeaderBuffer);
							tmpMsgIn.body.resize(tmpMsgIn.size());
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

static std::vector<uint8_t> _serialize(const message& msg)
{
	std::vector<uint8_t> buffer(sizeof(msg));
	std::memcpy(buffer.data(), &msg, sizeof(msg));
	return buffer;
}

static message_header _deserialize_header(const std::vector<uint8_t>& buffer)
{
	message_header header{};
	std::memcpy(&header, buffer.data(), sizeof(message_header));
	return header;
}
	
	
