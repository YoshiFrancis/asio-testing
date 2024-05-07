#include <cstdlib>
#include <deque>
#include <iostream>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include "asio.hpp"
#include "newmessage.hpp"

// typedef std::vector<uint8_t> message;
using asio::ip::tcp;

class Client
{
public:
  Client(asio::io_context& io_context, tcp::resolver::results_type& endpoints)
    : io_{io_context}, socket_(io_context)
  {
    Connect(endpoints);
  }

  void deliver(message message_)
  {
    bool is_curr_writing = !msgQ_.empty();
    msgQ_.push_back(message_);
    if (!is_curr_writing)
    {
      Write();
    }
  }

private:

  void Connect(tcp::resolver::results_type endpoints)
  {
    asio::async_connect(socket_, endpoints,
      [this](std::error_code ec, tcp::endpoint)
      {
        if (!ec)
        {
          std::cout << "Successfully connected!\n";
          ReadHeader();   // begin reading
        }
      });
  }

  void ReadHeader()
  {
    asio::async_read(socket_, asio::buffer(&buffer_.header_buffer, 4),
    [this](std::error_code ec, size_t len)
    {
      if (!ec)
      {
        std::cout << "READING HEADER: " << len << "\n";
        // buffer_.deserialize(buffer_.header_buffer);
        // std::cout << "Read header size: " << buffer_.header.size << "\n";
        // ReadBody();
      }
      else 
      {
        std::cout << "Reading Header Error: " << ec.message() << "\n";
      }
    });
  }

  void ReadBody()
  {
    buffer_.setSize(5);
    asio::async_read(socket_, asio::buffer(buffer_.body),
      [this](std::error_code ec, size_t len)
      {
        if (!ec)
        {
          std::string message_{};
          for (int i {0}; i < len; ++i)
          {
            message_ += buffer_.body[i];
          }
          std::cout << "Message received: " << message_ << "\n";
          ReadHeader();
        }
      });
  }

  void Write()
  {
    std::vector<uint8_t> buffer = msgQ_.front().serialize();
    asio::async_write(socket_, asio::buffer(&buffer, 4),
    [this](std::error_code ec, size_t len)
    {
      std::cout << "Bytes sent: " << len << "\n";
      if (!ec)
      {
        std::cout << "Let's see?\n";

        msgQ_.pop_front();      // causes bugs!!!! somehow freeing something
        std::cout << "Is it me?\n";
        if (!msgQ_.empty())
        {
          Write();
        }
      }
      else
      {
        std::cout << "Writing error: " << ec.message() << "\n";
      }
    });
  }

  asio::io_context& io_;
  tcp::socket socket_;
  std::deque<message> msgQ_;
  message buffer_;

};

int main(int argc, char* argv[])
{

  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: chat_client <host> <port>\n";
      return 1;
    }

    asio::io_context io_context;
    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], argv[2]);
    Client c(io_context, endpoints);
    message msg;
    msg.setSize(5);
    msg.body = {'h','e','l','l','o'};
    c.deliver(msg);
    io_context.run();

  }
  catch(const std::exception& e)
  {
    std::cerr << "Error" << e.what() << '\n';
  }
  return 1;
  
}
