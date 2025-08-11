cmake_minimum_required(VERSION 3.10)
project(GameServer)

set(CMAKE_CXX_STANDARD 14)

# Add source files
set(SOURCES
    main.cpp
    Room.cpp
    GameServer.cpp
)

# Add header files
set(HEADERS
    Room.h
    GameServer.h
)

# Create executable
add_executable(GameServer ${SOURCES} ${HEADERS})

# Link threads library (for mutex support)
find_package(Threads REQUIRED)
target_link_libraries(GameServer Threads::Threads)