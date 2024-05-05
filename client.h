#define ASIO_STANDALONE
#include "asio.hpp"
#include "chat_message.cpp"
#include "connection.cpp"
#include <memory>
#include <deque>

using asio::ip::tcp;
typedef std::shared_pointer<connection> connection_ptr;
typedef std::deque<message> message_queue;

class client 
{

public:
	client(asio::io_context io_context, tcp::endpoint& endpoints)
		: m_io_context { io_context }, m_socket{ io_context, endpoints }
		{
			readHeader();
		}

		void disconnect()
		{
			m_socket.close()
		}

		void deliver(message message)
		{
			m_msg_queue_out.push_back(std::move(message));
			if (m_msg_queue_out.empty())
			{
				write();
			}
		}

private:
	
	void readHeader()
	{
		async_read(m_socket, asio::buffer(m_tmp_msg_in.header.data(), sizeof(m_tmp_msg_in.header)),
			[this](std::error_code ec, size_t len)
			{
				if (!ec)
				{
					readBody();
				}
				else
				{
					disconnect();
					std::cout << "Disconnectiong from server due to reading error!\n";
				}
			});
	}

	void readBody()
	{
		async_read(m_socket, asio::buffer(m_tmp_msg_in.body.data(), m_tmp_msg_in.body.size()),
				[this](std::error_code ec, size_t len)
				{
					if (!ec)
				{
					m_msg_queue_in.push_back(std::move(m_tmp_msg_in));
					readHeader();
				}
				else
				{
					disconnect();
					std::cout << "Disconnectiong from server due to reading error!\n";
				}
			});
		
	}

	void write()
	{
		async_write(m_socket, asio::buffer(m_tmp_msg_out.front.body.data(), m_tmp_msg_out.front.body.size()),
			[this](std::error_code ec, size_t len)
			{
				if (!ec)
				{
					m_msg_queue_out.pop_front();
					write();
				}
				else
				{
					disconnect();
					std::cout << "Disconnecting from server due to sending error!\n";
				}
			});
	}

private:
	message_queue m_msg_queue_in{};
	message_queue m_msg_queue_out{};
	message m_tmp_msg_in;
	asio::io_context m_io_context;
	tcp::socket m_socket;
};
