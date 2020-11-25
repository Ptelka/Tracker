cmake_minimum_required(VERSION 3.0)

project(sqlite3)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/sqlite3.c
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} -ldl)