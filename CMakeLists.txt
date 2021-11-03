cmake_minimum_required(VERSION 3.20)

set(eol_task_VERSION_MAJOR 0)
set(eol_task_VERSION_MINOR 1)
set(eol_task_VERSION_PATCH 0)
set(eol_task_VERSION ${eol_task_VERSION_MAJOR}.${eol_task_VERSION_MINOR}.${eol_task_VERSION_PATCH})

project(eol_task 
    VERSION ${eol_task_VERSION}
    LANGUAGES CXX
)
set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

add_library(eol_task INTERFACE )
add_library(eol::task ALIAS eol_task)

target_include_directories(eol_task INTERFACE include/)

option(eol_task_TEST "build tests" TRUE)

if(eol_task_TEST)
    add_executable(eol_task_test test/test.cpp)
    target_link_libraries(
        eol_task_test 
        PUBLIC eol::task
    )
endif()
