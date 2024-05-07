#include <cstdlib>
#include <vector>

struct message_header
{
  uint32_t size;
};

struct message
{
  message_header header;
  std::vector<uint8_t> body;

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
};