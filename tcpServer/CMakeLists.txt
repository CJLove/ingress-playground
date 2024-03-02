cmake_minimum_required(VERSION 3.17)

project(tcp)

include_directories(
    ${CMAKE_SOURCE_DIR}/include/sockets-cpp
    .
)

# Threads
find_package(Threads)


set (serverApp_SRC serverApp.cpp)
add_executable( serverApp ${serverApp_SRC} )

target_link_libraries( serverApp PUBLIC Threads::Threads )

install(TARGETS serverApp DESTINATION tcpServer/bin/${BIN_PATH})