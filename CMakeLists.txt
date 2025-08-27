cmake_minimum_required(VERSION 3.10)
project(GameServer)

set(CMAKE_CXX_STANDARD 14)

# Set source and include directories
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add include directories
include_directories(${INCLUDE_DIR})

# Add source files organized by module
set(CORE_SOURCES
    ${SOURCE_DIR}/core/GameServer.cpp
)

set(GAME_SOURCES
    ${SOURCE_DIR}/game/Room.cpp
)

set(UTILS_SOURCES
    ${SOURCE_DIR}/utils/Logger.cpp
)

set(MAIN_SOURCES
    ${SOURCE_DIR}/Main.cpp
)

# Combine all sources
set(SOURCES
    ${CORE_SOURCES}
    ${GAME_SOURCES}
    ${UTILS_SOURCES}
    ${MAIN_SOURCES}
)

# Add header files
set(HEADERS
    ${INCLUDE_DIR}/core/GameServer.h
    ${INCLUDE_DIR}/game/Room.h
)

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Create executable
add_executable(GameServer ${SOURCES} ${HEADERS})

# Link threads library (for mutex support)
find_package(Threads REQUIRED)
target_link_libraries(GameServer Threads::Threads)

# For Linux-specific optimizations
if(UNIX AND NOT APPLE)
    target_link_libraries(GameServer rt)
endif()

# Windows-specific libraries
if(WIN32)
    target_link_libraries(GameServer ws2_32)
    add_definitions(-D_WIN32_WINNT=0x0600)
endif()

# Set output directories
set_target_properties(GameServer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)