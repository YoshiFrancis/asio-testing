#include <iostream>
#include <vector>
#include <string>

#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP


typedef uint8_t flag; // flag to indicate which type of action the user is doing
											// flags that can be used:
											// P - play a card
											// H - ask for instructions
											// L - leave game
											// C - chat in the chat room
											// J - join the game

struct message_header
{
	flag flag {};
	uint32_t size = 0; // size of the body data
};

struct message
{
	message_header header{};
	std::vector<uint8_t> body{};

	int size()
	{
		return body.size();
	}

	friend std::ostream& operator<< (std::ostream& os, const message& msg)
	{
		os << "FLAG: " << msg.header.flag << " || Size: " << msg.header.size;
		return os;
	}

	friend message& operator<< (message& msg, std::string& text) 
	{
		size_t idx = msg.body.size();
		msg.body.resize(msg.body.size() + text.length());
		std::memcpy(msg.body.data() + idx, text.data(), text.length());
		msg.header.size = msg.size();
		return msg;
	}

	friend message& operator>> (message& msg, std::string& str_holder)
	{
		std::string tmp(msg.body.begin(), msg.body.end());
		str_holder = std::move(tmp);
		msg.body.clear();
		return msg;
	}
};

#endif // CHAT_MESSAGE_HPP
