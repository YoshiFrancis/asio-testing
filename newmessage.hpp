#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>

struct message_header
{
  uint32_t size;
};

struct message
{
  message_header header;
  std::vector<uint8_t> body;
  std::vector<uint8_t> header_buffer;

  int header_size()
  {
    return sizeof(header);
  }

  int body_size()
  {
    return body.size();
  }

  void setSize(int new_size)
  {
    header.size = new_size;
    body.resize(new_size);
  }

  std::vector<uint8_t>& serialize()
  {
    // std::string bodyStr(body.begin(), body.end());
    // std::string sizeStr = std::to_string(header.size);
    // std::cout << sizeStr <<  "," << bodyStr << "\n";
    // return sizeStr + "," + bodyStr;
    header_buffer.resize(4);
    std::memcpy(&header_buffer, &header.size, sizeof(header.size));
    return header_buffer;
  }

  void deserialize(std::vector<uint8_t> buffer)
  {
    // size_t first = str.find(",");
    // std::string sizeStr = str.substr(0, first);
    // header.size = std::stoi(sizeStr);
    // std::string bodyStr = str.substr(first + 1, str.size());
    // std::cout << sizeStr << "," << bodyStr << "\n";
    // body.assign(bodyStr.begin(), bodyStr.end());
    std::memcpy(&header.size, &buffer, buffer.size());
  }
};