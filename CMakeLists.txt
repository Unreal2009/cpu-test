cmake_minimum_required(VERSION 3.25)

project(cpu_test C)

set(CMAKE_C_STANDARD 99)

add_executable(${CMAKE_PROJECT_NAME} lib/utils.c lib/utils.h lib/rasp_tools.c lib/rasp_tools.h)

target_sources(${CMAKE_PROJECT_NAME}
        PRIVATE
        main.c
        )

set(CPU_TEST_LIB_SOURCE
        lib/utils.c
        lib/rasp_tools.c
        )

target_sources(${CMAKE_PROJECT_NAME}
        PRIVATE
        ${CPU_TEST_LIB_SOURCE}
        )

target_include_directories(${CMAKE_PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/
        )

message("CMAKE_CURRENT_SOURCE_DIR : " ${CMAKE_CURRENT_SOURCE_DIR})
#message("CMAKE_PROJECT_NAME : " ${CMAKE_PROJECT_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99 -pthread" )