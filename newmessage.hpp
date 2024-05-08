#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <cstdint>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iterator>

class message
{
public:
  static constexpr std::size_t header_length = 4;
  std::string data_;


  message()
    : body_length_(0)
  {
  }

  const std::string* data() const
  {
    return &data_;
  }

  std::string* data()
  {
    return &data_;
  }

  std::size_t length() const
  {
    return header_length + body_length_;
  }

  const auto body() const
  {
    return std::next(data_.begin(), header_length);
  }

  auto body()
  {
    return std::next(data_.begin(), header_length);
  }

  std::size_t body_length() const
  {
    return body_length_;
  }

  void body_length(std::size_t new_length)
  {
    body_length_ = new_length;
    data_.resize(body_length_);
  }

  void decode_header()
  {
    std::string header = data_.substr(0, 4);
    body_length_ = std::stoi(header);
  }

  void encode_header()
  {
    std::string header(header_length, ' ');
    std::sprintf(&header[0], "%4d", static_cast<int>(body_length_));
    data_ = header + data_;
    std::cout << "Message being encoded and sent! " << data_ << "\n";
  }

private:
  std::size_t body_length_;
};

// TODO
// SEND THE BODY DATA ALONG WITH THE HEADER DATA