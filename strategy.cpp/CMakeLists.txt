
cmake_minimum_required(VERSION 3.19)

# project
project(strategy_cpp)

message(STATUS " ")
message(STATUS ${PROJECT_NAME})
message(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")

if (WIN32)
	message(STATUS "########### os is windows ###########")
	include_directories(
		"${CMAKE_CURRENT_SOURCE_DIR}/../ctp.cpp" 
		"${CMAKE_CURRENT_SOURCE_DIR}/../libs/rapidjson/include" 
		"${CMAKE_CURRENT_SOURCE_DIR}/../libs/ctpapi/6.3.15_20190220_tradeapi64_se_windows"
		#"${CMAKE_CURRENT_SOURCE_DIR}/../libs/ctpapi/6.3.15_20190220_tradeapi_se_windows"
	)
	link_directories(
		"${CMAKE_CURRENT_SOURCE_DIR}/../libs/ctpapi/6.3.15_20190220_tradeapi64_se_windows"
		#"${CMAKE_CURRENT_SOURCE_DIR}/../libs/ctpapi/6.3.15_20190220_tradeapi_se_windows"
	)

elseif(UNIX)	
	message(STATUS "########### os is UNIX-like os's ###########")
endif()

file(GLOB strategy_file
	Strategy*.h 
	Strategy*.cpp)
message(STATUS ${strategy_file})

#add_executable(strategy.cpp ${strategy_file} main.cpp)
#target_link_libraries(
#	strategy.cpp
#	ctp_cpp
#	thostmduserapi_se.lib 
#	thosttraderapi_se.lib
#)

ADD_LIBRARY(${PROJECT_NAME} ${strategy_file})
ADD_LIBRARY(${PROJECT_NAME}_static STATIC ${strategy_file})

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/MP")
SET_TARGET_PROPERTIES(${PROJECT_NAME}_static PROPERTIES COMPILE_FLAGS "/MP")

