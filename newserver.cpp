#include <cstdlib>
#include <deque>
#include <iostream>
#include <memory>
#include <set>
#include <utility>
#include "asio.hpp"
#include "newmessage.hpp"


using asio::ip::tcp;
// typedef std::vector<uint8_t> message;
class Connection;

class ConnectionI 
{
public:
	virtual ~ConnectionI() {}
	virtual void deliver(message msg) = 0;
};

typedef std::shared_ptr<ConnectionI> conn_ptr;

class Room
{
public:

  void join(conn_ptr conn)
  {
    std::cout << "Someone has joined!\n";
    connections_.insert(conn);
  }

  void leave(conn_ptr conn)
  {
    connections_.erase(conn);
  }

  void deliverAll(message& msg)
  {
    msg.encode_header();
    std::cout << connections_.size() << " number of connections to send to\n";
    for (auto conn : connections_)
    {
      conn->deliver(msg);
    }
  }


private:
  std::set<conn_ptr> connections_;
};

class Connection
  : public std::enable_shared_from_this<Connection>,
		public ConnectionI
{
public:
  Connection(tcp::socket socket, Room& room)
    : socket_{std::move(socket)}, room_{room}
  {
  }

  void start()
  {
    room_.join(shared_from_this());
    ReadHeader();
  }

  void deliver(message msg)
  {
    bool is_curr_writing = !messageQ_.empty();
    messageQ_.push_back(msg);
    if (!is_curr_writing)
    {
      Write();
    }
  }

private:
  tcp::socket socket_;
  Room& room_;
  std::deque<message> messageQ_;
  message buffer_;

  void ReadHeader()
  {
    auto self(shared_from_this());
    buffer_.data_.resize(4);    // size of the header
    asio::async_read(socket_, asio::buffer(buffer_.data(), 4),
    [this, self](std::error_code ec, size_t len)
    {
      if (!ec)
      {
        buffer_.decode_header();
        ReadBody();
      }
      else 
      {
        std::cout << "Reading Header Error: " << ec.message() << "\n";
      }
    });
  }
  void ReadBody()
  {
    auto self(shared_from_this());
    asio::async_read(socket_, asio::buffer(buffer_.data(), buffer_.body_length()),
    [this, self](std::error_code ec, size_t len)
    {
      if (!ec)
      {
        std::cout << buffer_.data_ << "\n";
        room_.deliverAll(buffer_);
        ReadHeader();
      } 
      else
      {
        std::cout << "Reading error: " << ec.message() << "\n";
        room_.leave(shared_from_this());
      }
    });
  }

  void Write()
  {
    auto self(shared_from_this());
    asio::async_write(socket_, asio::buffer(messageQ_.front().data_),
    [this, self](std::error_code ec, size_t)
    {
      if (!ec)
      {
        messageQ_.pop_front();
        if (!messageQ_.empty())
        {
          Write();
        }
      }
      else
      {
        std::cout << "Write error: " << ec.message() << "\n";
        room_.leave(shared_from_this());
      }
    });
  }
};

class Server
{
public:
  Server(asio::io_context& io_context, tcp::endpoint& endpoint)
    : acceptor_(io_context, endpoint)
  {
    Accept();
  }

  void Accept()
  {
    acceptor_.async_accept(
      [this](std::error_code ec, tcp::socket socket)
      {
        if (!ec)
        {
          std::cout << "Successful connection!\n";
          std::make_shared<Connection>(std::move(socket), room_)->start();
          Accept();
        }
      });
  }
private:
  tcp::acceptor acceptor_;
  Room room_;
};

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cout << "Usage: <port>\n";
    }

    asio::io_context io_context;
    tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[1]));
    Server server(io_context, endpoint);
    io_context.run();
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}
