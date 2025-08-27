# Test configuration for Game Server
# This file will be included by the main CMakeLists.txt when testing is enabled

# Enable testing
enable_testing()

# Add test executable
add_executable(GameServerTests
    test_main.cpp
    test_room.cpp
    test_gameserver.cpp
)

# Link test libraries
target_link_libraries(GameServerTests
    GameServer
    Threads::Threads
)

# Add tests
add_test(NAME RoomTests COMMAND GameServerTests)
add_test(NAME GameServerTests COMMAND GameServerTests)

# Set test properties
set_tests_properties(RoomTests GameServerTests PROPERTIES
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
