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
# In CMakeLists.txt
if(WIN32)
    target_link_libraries(GameServer ws2_32)
    add_definitions(-D_WIN32_WINNT=0x0600)
endif()