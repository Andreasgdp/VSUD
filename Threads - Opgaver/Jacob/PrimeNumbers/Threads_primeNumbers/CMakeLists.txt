cmake_minimum_required(VERSION 3.5)

project(Threads_primeNumbers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads)
add_executable(Threads_primeNumbers main.cpp)
target_link_libraries(Threads_primeNumbers ${CMAKE_THREAD_LIBS_INIT})
