
cmake_minimum_required(VERSION 3.19)

# project
project(wind.event)

message(STATUS " ")
message(STATUS ${PROJECT_NAME})
message(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")


set(wind.event
	wevent.cpp 
	)
	
add_executable(wind.event ${wind.event})

