cmake_minimum_required(VERSION 3.29) 
set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_CXX_STANDARD 20)
project(testing)

include_directories(~/Documents/ASIO/asio-1.30.2/include chat_message.hpp connection.hpp game_room.hpp)

add_executable(main main.cpp)

