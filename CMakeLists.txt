cmake_minimum_required(VERSION 3.2)

project(socket_comm_rem)

find_package(OpenCV REQUIRED)
set(CMAKE_PREFIX_PATH /home/linux/SFML-2.5.1/lib/cmake/SFML)
find_package(SFML 2.5 COMPONENTS graphics system window)
find_package(Boost COMPONENTS iostreams serialization REQUIRED)

set(SOCKET_COMM_SRC
    socket_comm.h
    socket_comm.cpp
)

set(DATA_TYPES_SRC
    data_types.h
    data_types.cpp
)

add_library(socket_comm SHARED STATIC ${SOCKET_COMM_SRC})
target_link_libraries(socket_comm sfml-graphics sfml-system sfml-window ${OpenCV_LIBRARIES} ${Boost_LIBRARIES})

add_executable(remote_robot remote_robot.cpp)
target_link_libraries(remote_robot ${OpenCV_LIBRARIES} ${Boost_LIBRARIES})

add_executable(remote_computer remote_computer.cpp)
target_link_libraries(remote_computer socket_comm)

add_executable(user_input user_input.cpp)
target_link_libraries(user_input socket_comm)
