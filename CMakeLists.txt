cmake_minimum_required(VERSION 3.2)

project(socket_comm_rem)

find_package(OpenCV REQUIRED)

add_executable(server server.cpp)
target_link_libraries(server ${OpenCV_LIBRARIES})
add_executable(client client.cpp)
target_link_libraries(client ${OpenCV_LIBRARIES})
