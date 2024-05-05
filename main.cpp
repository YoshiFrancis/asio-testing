#include "chat_message.hpp"
#include "connection.hpp"
#include "game_room.hpp"
#include <iostream>


int main(int argc, char* argv[]) 
{

	message msg;
	msg.header.flag = 'P';
	std::string testing { "I am Yoshi" };
	msg << testing;
	std::string testing_str;
	std::cout << msg << "\n";
	msg >> testing_str;
	std::cout << testing_str << "\n";
	
	return 1;
}
