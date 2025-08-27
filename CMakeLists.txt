cmake_minimum_required(VERSION 3.10)
project(snake CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
	src/main.cpp
	src/structures_impl.cpp
)
set(CMAKE_BUILD_TYPE Debug)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)

add_subdirectory(external/raylib)
add_subdirectory(external/fmt)

target_link_libraries(${PROJECT_NAME} raylib)
target_link_libraries(${PROJECT_NAME} fmt)

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} m)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()
