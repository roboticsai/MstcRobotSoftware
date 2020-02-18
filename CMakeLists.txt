cmake_minimum_required(VERSION 3.2)

project(socket_comm_rem)

find_package(OpenCV REQUIRED)
set(CMAKE_PREFIX_PATH /home/dinesh/SFML-2.5.1/lib/cmake/SFML)
find_package(SFML COMPONENTS graphics window system)

add_executable(remote_computer remote_computer.cpp)
target_link_libraries(remote_computer ${OpenCV_LIBRARIES})
add_executable(remote_robot remote_robot.cpp)
target_link_libraries(remote_robot ${OpenCV_LIBRARIES})

add_executable(mouse_movement mouse_movement.cpp)
target_link_libraries(mouse_movement ${OpenCV_LIBRARIES})
add_executable(key_press key_press.cpp)
target_link_libraries(key_press ${OpenCV_LIBRARIES})

include_directories(${SFML_INCLUDE_DIR})
add_executable(rem_rob_aud rem_rob_aud.cpp)
target_link_libraries(rem_rob_aud sfml-graphics sfml-audio)

