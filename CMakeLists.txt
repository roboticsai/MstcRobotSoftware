cmake_minimum_required(VERSION 3.2)

project(socket_comm_rem)

find_package(OpenCV REQUIRED)
find_package(X11 REQUIRED)
find_package (Threads)

include_directories(${X11_INCLUDE_DIR})

add_executable(server server.cpp)
target_link_libraries(server ${OpenCV_LIBRARIES})
add_executable(client client.cpp)
target_link_libraries(client ${OpenCV_LIBRARIES})

add_executable(mouse_movement mouse_movement.cpp)
target_link_libraries(mouse_movement ${OpenCV_LIBRARIES} ${X11_LIBRARIES})
add_executable(key_press key_press.cpp)
target_link_libraries(key_press ${OpenCV_LIBRARIES})

target_link_libraries (mouse_movement ${CMAKE_THREAD_LIBS_INIT})
