#include "connection.h"

/*
static std::vector<uint8_t> _serialize(const message& msg);
static message_header _deserialize_header(const std::vector<uint8_t>& buffer);
*/

void connection::disconnect()
{
    m_room.leave(shared_from_this());
}

void connection::deliver(message msg)
{
	std::cout << "delivering msg to connection!\n";
	msg_queue_out.push_back(std::move(msg));
	if (msg_queue_out.empty())
	{
			write();
	}
}

void connection::start()
{
	readBody();
}

void connection::write()
{
	auto self(shared_from_this());
	async_write(connection::m_socket, asio::buffer(msg_queue_out.front().body.data(), sizeof(msg_queue_out.front().body.size())),
			[this, self](const std::error_code& ec, size_t len)
			{
				std::cout << "I just wrote some data...\n";
				if (!ec)
				{
						msg_queue_out.pop_front();
						write();
				}
				else
				{
					std::cout << "Writing: " << ec.message() << "\n";
					disconnect();
				}
			});
}
/*
void connection::readHeader()
{
	std::cout << "Begin reading for header\n";
	auto self(shared_from_this());
	tmpMsgHeaderBuffer.resize(sizeof(message_header));
	async_read(m_socket, asio::buffer(tmpMsgHeaderBuffer.data(), sizeof(message_header)),
			[this, self](std::error_code ec, size_t len)
			{
				if (!ec)
				{
					tmpMsgIn.header = _deserialize_header(tmpMsgHeaderBuffer);
					tmpMsgIn.body.resize(tmpMsgIn.header.size);
					std::cout << tmpMsgIn.header.size << " " << tmpMsgIn.size() << " " << tmpMsgIn.header.flag << " " << len << "\n";
					readBody();
				}
				else
				{
					std::cout << "Reading Header: " << ec.message() << "\n";
					disconnect();
				}
			});
}
*/
void connection::readBody()
{
    auto self(shared_from_this());
    async_read(m_socket, asio::buffer(_buffer.data(), 5),
        [this, self](std::error_code ec, size_t len)
        {
					std::cout << "Bytes read: " << len << "\n";
					std::cout << "Message received: ";
					for (auto c : _buffer)
						std::cout << c;
					std::cout << "\n";
					if (!ec)
					{
						std::cout << "Primed for reading again...\n";
						readBody();
					}
					else
					{
						std::cout << "ReadBody: " << ec.message() << "\n";
						disconnect();
					}
        });
}
/*
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
	
*/	
