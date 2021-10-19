cmake_minimum_required(VERSION 3.5)

project(Asynkorne_Hooligans LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_executable(Asynkorne_Hooligans main.cpp)
find_package(Threads REQUIRED)
target_link_libraries(Asynkorne_Hooligans PRIVATE Threads::Threads)
