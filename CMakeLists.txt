cmake_minimum_required(VERSION 3.2)

project(socket_comm_rem)

find_package(OpenCV REQUIRED)
set(CMAKE_PREFIX_PATH /home/sfml/SFML-2.5.1/lib/cmake/SFML)
find_package(SFML COMPONENTS graphics audio window system network graphics)

set(SOURCE_FILES
    common_header.h
    common_header.cpp
)

add_library(deye SHARED STATIC ${SOURCE_FILES})
target_link_libraries(deye sfml-graphics sfml-system sfml-window sfml-audio ${OpenCV_LIBRARIES})

add_executable(remote_robot remote_robot.cpp)
target_link_libraries(remote_robot deye)

add_executable(remote_computer remote_computer.cpp)
target_link_libraries(remote_computer deye)

add_executable(user_input user_input.cpp)
target_link_libraries(user_input deye)
