cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME ECS_LIB)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE_FILES
        "include/*.hpp"
        "src/*.cpp"
        )


ADD_LIBRARY(${PROJECT_NAME} ${SOURCE_FILES})

ADD_EXECUTABLE(Test tests/main.cpp)
target_link_libraries(Test ECS_LIB)

include_directories(include)