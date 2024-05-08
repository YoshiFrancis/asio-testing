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

  const std::string* body() const
  {
    return &data_ + header_length;
  }

  std::string* body()
  {
    return &data_ + header_length;
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
    data_.resize(body_length_);
  }

  void encode_header()
  {
    std::string header(header_length, ' ');
    body_length_ = data_.length();
    std::sprintf(&header[0], "%4d", static_cast<int>(body_length_));
    data_ = header + data_;
  }

private:
  std::size_t body_length_;
};

// TODO
// SEND THE BODY DATA ALONG WITH THE HEADER DATA