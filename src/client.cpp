#include "client.h"


void client::connect(tcp::resolver::results_type& endpoint)
{
	async_connect(m_socket, endpoint, 
		[this](const std::error_code ec, tcp::endpoint)
		{
			if (!ec)
			{
				readHeader();
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
    async_read(m_socket, asio::buffer(&m_tmp_msg_in.header, sizeof(m_tmp_msg_in.header)),
        [this](std::error_code ec, size_t len)
        {
					if (!ec)
					{
						readBody();
					}
					else
					{
						disconnect();
						std::cout << "Disconnecting from server due to reading error!\n";
					}
        });
}

void client::readBody()
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
					std::cout << "Disconnecting from server due to reading error!\n";
				}
			});
}

void client::write()
{
	async_write(m_socket, asio::buffer(m_msg_queue_out.front().body.data(), m_msg_queue_out.front().body.size()),
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
