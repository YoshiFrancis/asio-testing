include_directories(~/Documents/ASIO/asio-1.30.2/include)
add_library(server STATIC connection.cpp game_room.cpp server.cpp)
add_library(client STATIC client.cpp connection.cpp)
target_include_directories(server PUBLIC ${PROJECT_SOURCE_DIR/include})
