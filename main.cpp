#include "chat_message.hpp"
#include "connection.h"
#include "game_room.h"
#include <iostream>


std::vector<uint8_t> serialize(const message& message)
{
	std::vector<uint8_t> buffer (sizeof(message));
	std::memcpy(buffer.data(), &message, sizeof(message));
	return buffer;
}

message_header deserialize_header(const std::vector<uint8_t>& buffer)
{
	message_header header{};
	std::memcpy(&header, buffer.data(), sizeof(message_header));
	return header;
}


int main(int argc, char* argv[]) 
{

	message msg;
	msg.header.flag = 'P';
	std::string testing { "I am Yoshi" };
	msg << testing;
	std::string testing_str;
	std::cout << msg << "\n";
	auto buffer = serialize(msg);
	for (auto c : buffer)
		std::cout << c;
	std::cout << "\n";
	message_header header = deserialize_header(buffer);
	std::cout << header.flag << " " << header.size << "\n";
	
	return 1;
}
