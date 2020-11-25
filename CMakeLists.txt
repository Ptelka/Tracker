cmake_minimum_required(VERSION 3.0)
project(Tracker)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)
find_package(Threads REQUIRED)

set(SOURCE_FILES ${SOURCE_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/database/Connection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/database/Statement.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/filesystem/inotify/DirectoryWatcher.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/filesystem/inotify/DirectoryWatchers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/filesystem/inotify/Inotify.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/filesystem/inotify/InotifyWatcher.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/process/Pid.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/logger/Logger.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/worker/Worker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/worker/MonitorFiles.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/worker/MonitorProcesses.cpp
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/sqlite3)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/sqlite3)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/rapidjson)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/rapidjson/include)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(${PROJECT_NAME} sqlite3 Threads::Threads)