cmake_minimum_required(VERSION 3.2)

project(socket_comm_rem)

find_package(OpenCV REQUIRED)
find_package(X11 REQUIRED)
find_package (Threads)

include_directories(${X11_INCLUDE_DIR})

add_executable(remote_computer remote_computer.cpp)
target_link_libraries(remote_computer ${OpenCV_LIBRARIES})
add_executable(remote_robot remote_robot.cpp)
target_link_libraries(remote_robot ${OpenCV_LIBRARIES})

add_executable(mouse_movement mouse_movement.cpp)
target_link_libraries(mouse_movement ${OpenCV_LIBRARIES} ${X11_LIBRARIES})
add_executable(key_press key_press.cpp)
target_link_libraries(key_press ${OpenCV_LIBRARIES})

