cmake_minimum_required(VERSION 3.2)

project(socket_comm_rem)

find_package(OpenCV REQUIRED)
set(CMAKE_PREFIX_PATH /home/linux/SFML-2.5.1/lib/cmake/SFML)
find_package(SFML 2.5 COMPONENTS audio network graphics system window)
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

add_executable(jaguar_s1 jaguar_s1.cpp Server.cpp)
target_link_libraries(jaguar_s1 ${OpenCV_LIBRARIES} ${Boost_LIBRARIES} sfml-audio sfml-network)

add_executable(jaguar_s1_controller jaguar_s1_controller.cpp Server.cpp)
target_link_libraries(jaguar_s1_controller socket_comm sfml-audio sfml-network)

add_executable(user_input user_input.cpp)
target_link_libraries(user_input socket_comm)

add_executable(server server.cpp Server.cpp)
target_link_libraries(server sfml-audio sfml-network)

add_executable(client client.cpp Server.cpp)
target_link_libraries(client sfml-audio sfml-network)

add_executable(arduino_serial arduino_serial.cpp)

