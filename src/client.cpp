#include "client.h"
/*
static std::vector<uint8_t> _serialize(const message& message);
static message_header _deserialize_header(const std::vector<uint8_t>& buffer);
*/
void client::connect(tcp::resolver::results_type& endpoint)
{
	async_connect(m_socket, endpoint, 
		[this](const std::error_code ec, tcp::endpoint)
		{
			if (!ec)
			{
				std::cout << "Connected\n";
				readBody();
			}
			else 
			{
				std::cout << "Error connecting!\n";
			}
		});
}

void client::disconnect()
{
		asio::post(m_io_context, [this]() { m_socket.close(); });
}

void client::deliver(message message)
{
	message.header.flag = 'P';
	asio::post(m_io_context, 
		[this, message]()
		{
			bool is_currently_writing = !m_msg_queue_out.empty();
			m_msg_queue_out.push_back(std::move(message));
			if (!is_currently_writing)
			{
				write();
			}
		});
}

void client::readHeader()
{
		std::vector<uint8_t> buffer(sizeof(message_header));
    async_read(m_socket, asio::buffer(buffer.data(), sizeof(message_header)),
        [this, &buffer](std::error_code ec, size_t len)
        {
					std::cout << len << " bytes read\n";
					if (!ec)
					{
						//m_tmp_msg_in.header = _deserialize_header(buffer);
						m_tmp_msg_in.body.resize(m_tmp_msg_in.size());
						readBody();
					}
					else
					{
						std::cout << "Disconnecting from server due to reading header error!\n";
						std::cout << ec.message() << "\n";
						m_socket.close();
					}
        });
}

void client::readBody()
{
	async_read(m_socket, asio::buffer(&_buffer, 5),
			[this](std::error_code ec, size_t len)
			{
				if (!ec)
				{
					std::cout << "Read in body!\n";
					readBody();
				}
				else
				{
					std::cout << "Disconnecting from server due to reading body error!\n";
					std::cout << ec.message() << "\n";
					m_socket.close();
				}
			});
}

void client::write()
{
	_buffer = { 'B', 'e', 'l', 'l', 'o' };
	async_write(m_socket, asio::buffer(_buffer, _buffer.size()),
			[this](std::error_code ec, size_t len)
			{
			std::cout << "Sending!\n";
			if (!ec)
			{
				m_msg_queue_out.pop_front();
				if (!m_msg_queue_out.empty())
				{
					write();
				}
			}
			else
			{
				std::cout << "Disconnecting from server due to sending error!\n";
				m_socket.close();
			}
	});
}
/*
static std::vector<uint8_t> _serialize(const message& message)
{
	std::vector<uint8_t> buffer(sizeof(message));
	std::memcpy(buffer.data(), &message, sizeof(message));
	return buffer;
}

static message_header _deserialize_header(const std::vector<uint8_t>& buffer)
{
	message_header header{};
	std::memcpy(&header, buffer.data(), sizeof(header));
	return header;
}
*/
