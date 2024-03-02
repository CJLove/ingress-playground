cmake_minimum_required(VERSION 3.17)

project(examples)

include_directories(
    ${CMAKE_SOURCE_DIR}/include/sockets-cpp
    .
)

# Threads
find_package(Threads)


set (unicastApp_SRC unicastApp.cpp)
add_executable( unicastApp ${unicastApp_SRC} )

target_link_libraries( unicastApp PUBLIC Threads::Threads )

install(TARGETS unicastApp DESTINATION udpServer/bin/${BIN_PATH})