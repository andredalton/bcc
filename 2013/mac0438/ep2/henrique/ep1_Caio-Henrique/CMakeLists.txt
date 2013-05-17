CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(paralela-ep1 C)

find_package(OpenMP REQUIRED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

find_package(Threads REQUIRED)

add_definitions(-O3)

add_executable(parte1 parte1.c)
add_executable(parte2 parte2.c)
add_executable(parte3 parte3.c)

target_link_libraries (parte3 ${CMAKE_THREAD_LIBS_INIT})
