
cmake_minimum_required(VERSION 3.19)

# project
project(ctp_cpp)

message(STATUS " ")
message(STATUS ${PROJECT_NAME})
message(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")

if (WIN32)
	message(STATUS "########### os is windows ###########")
	include_directories(
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



set(ctp.cpp 
	Run.cpp 
	CtpMD.h CtpMD.cpp 
	CtpTD.h CtpTD.cpp 
	DataStruct.h DataStruct.cpp
	StrategyTemplate.h StrategyTemplate.cpp
	MDEvent.h MDEvent.cpp
	TDEvent.h TDEvent.cpp
	Indicators.h Indicators.cpp
	)
	
set(ctp_file 
	CtpMD.h
	CtpMD.cpp 
	CtpTD.h
	CtpTD.cpp 
	DataStruct.h
	DataStruct.cpp
	StrategyTemplate.h
	StrategyTemplate.cpp
	MDEvent.h
	MDEvent.cpp
	TDEvent.h
	TDEvent.cpp
	Indicators.h
	Indicators.cpp
	)
	
	
add_executable(ctp.cpp ${ctp.cpp})

target_link_libraries(
	ctp.cpp
	thostmduserapi_se.lib 
	thosttraderapi_se.lib
)


ADD_LIBRARY(${PROJECT_NAME} ${ctp_file})
ADD_LIBRARY(${PROJECT_NAME}_static STATIC ${ctp_file})

IF (WIN32)
    MESSAGE(STATUS "###########################OS is windows###########################")

    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/MP")
    SET_TARGET_PROPERTIES(${PROJECT_NAME}_static PROPERTIES COMPILE_FLAGS "/MP")

ELSEIF (UNIX)
    MESSAGE(STATUS "###########################OS is UNIX-like OS's.###########################")

ENDIF ()