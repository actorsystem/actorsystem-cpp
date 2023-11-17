cmake_minimum_required(VERSION 3.15)
project(cpp_rabbi)

#include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)


#include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#conan_basic_setup(TARGETS)

find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(rabbitmq-c CONFIG REQUIRED)

add_executable(cpp_rabbi main.cpp amqp_helper.cpp amqp_consumer.cpp actor.cpp)
target_link_libraries(cpp_rabbi Boost::system Boost::filesystem rabbitmq::rabbitmq-static)
