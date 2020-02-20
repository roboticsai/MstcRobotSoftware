cmake_minimum_required(VERSION 3.2)

project(socket_comm_rem)

find_package(OpenCV REQUIRED)
set(CMAKE_PREFIX_PATH /home/linux/SFML-2.5.1/lib/cmake/SFML)
find_package(SFML COMPONENTS graphics audio window system network)

add_executable(remote_computer remote_computer.cpp)
target_link_libraries(remote_computer ${OpenCV_LIBRARIES})
add_executable(remote_robot remote_robot.cpp)
target_link_libraries(remote_robot ${OpenCV_LIBRARIES})

add_executable(rem_rob_aud rem_rob_aud.cpp)
target_link_libraries(rem_rob_aud sfml-graphics sfml-audio)

add_executable(user_input user_input.cpp)
target_link_libraries(user_input sfml-graphics sfml-audio ${OpenCV_LIBRARIES})

add_executable(audio_socket Sockets.cpp TCP.cpp UDP.cpp)
target_link_libraries(audio_socket sfml-graphics sfml-audio sfml-window sfml-network)

