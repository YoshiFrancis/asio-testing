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
    asio::post(io_, 
      [this, message_]
      {
        bool is_curr_writing = !msgQ_.empty();
        msgQ_.push_back(message_);
        if (!is_curr_writing)
        {
          Write();
        }
      });
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
    buffer_.data_.resize(4);    // size of the header
    asio::async_read(socket_, asio::buffer(buffer_.data(), 4),
    [this](std::error_code ec, size_t len)
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
    asio::async_read(socket_, asio::buffer(buffer_.data(), buffer_.body_length()),
      [this](std::error_code ec, size_t len)
      {
        if (!ec)
        {
          std::cout << "Message received: " << buffer_.data_ << "\n";
          ReadHeader();
        }
        else
        {
          std::cout << "Reading Body error: " << ec.message() << "\n";
          socket_.close();
        }
      });
  }

  void Write()
  {
    asio::async_write(socket_, asio::buffer(msgQ_.front().data_),
    [this](std::error_code ec, size_t len)
    {
      if (!ec)
      {
        msgQ_.pop_front();
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
    std::thread t([&io_context](){ io_context.run(); });
    std::string input{};
    while (std::getline(std::cin, input))
    {
      message msg;
      msg.data_ = input;
      msg.encode_header();
      c.deliver(msg);
    }
    t.join();
  }
  catch(const std::exception& e)
  {
    std::cerr << "Error" << e.what() << '\n';
  }
  return 1;
  
}
