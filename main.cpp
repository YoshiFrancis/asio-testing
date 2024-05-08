// #include "chat_message.hpp"
#include "newmessage.hpp"
// #include "connection.h"
// #include "game_room.h"
#include <iostream>
#include <vector>


// std::vector<uint8_t> serialize(const message& message)
// {
// 	std::vector<uint8_t> buffer (sizeof(message));
// 	std::memcpy(buffer.data(), &message, sizeof(message));
// 	return buffer;
// }

// message_header deserialize_header(const std::vector<uint8_t>& buffer)
// {
// 	message_header header{};
// 	std::memcpy(&header, buffer.data(), sizeof(message_header));
// 	return header;
// }

// std::vector<uint8_t> serialize(message_header header, std::vector<uint8_t>& header_buffer)
// {
// 	header_buffer.resize(sizeof(header.size));
// 	std::memcpy(header_buffer.data(), &header.size, sizeof(header.size));
// 	return header_buffer;
// }

// void deserialize_header(message_header header, std::vector<uint8_t>& header_buffer)
// {
// 	std::cout << "Trying to deserialize here: \n";
// 	std::memcpy(&header.size, header_buffer.data(), sizeof(header.size));
// 	std::cout << header.size << "\n";
// 	std::cout << "Passed deserialization\n";
// }


int main(int argc, char* argv[]) 
{

	// message msg;
	// msg.header.flag = 'P';
	// std::string testing { "I am Yoshi" };
	// msg << testing;
	// std::string testing_str;
	// std::cout << msg << "\n";
	// auto buffer = serialize(msg);
	// for (auto c : buffer)
	// 	std::cout << c;
	// std::cout << "\n";
	// message_header header = deserialize_header(buffer);
	// std::cout << header.flag << " " << header.size << "\n";
	// message msg;
	// msg.header.size = 100;
	// std::vector<uint8_t> header_buffer(4);
	// auto buffer = serialize(msg.header, header_buffer);
	// deserialize_header(msg.header, buffer);
	
	return 1;
}
