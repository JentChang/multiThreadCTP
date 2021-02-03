
cmake_minimum_required(VERSION 3.19)

# project
project(test.thread)

message(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")

if (WIN32)
	message(STATUS "########### os is windows ###########")
elseif(UNIX)	
	message(STATUS "########### os is UNIX-like os's ###########")
endif()


set(test_thread 
	run.cpp 
	mainEvent.cpp mainEvent.h 
	read.h read.cpp
	write.h write.cpp
	)
add_executable(test_thread ${test_thread})
