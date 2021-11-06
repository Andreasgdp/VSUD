cmake_minimum_required(VERSION 3.5)

project(Portefolje_2_DB_og_Traade LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads)
find_package(Qt6 COMPONENTS Sql REQUIRED)
add_executable(Portefolje_2_DB_og_Traade main.cpp
                                         NumberPlateGenerator.h NumberPlateGenerator.cpp
                                         Database.h Database.cpp)
target_link_libraries(Portefolje_2_DB_og_Traade ${CMAKE_THREAD_LIBS_INIT} Qt6::Sql)
