# Game Server

A cross-platform multiplayer game server built in C++ with networking capabilities, room management, and player handling.

## Features

- **Cross-platform Support**: Windows and Linux compatibility
- **Network Server**: TCP socket-based communication on configurable ports
- **Room Management**: Create, delete, and manage game rooms with player limits
- **Player Management**: Add/remove players from rooms, track player states
- **Game Logic**: Start games, handle game states, and manage room lifecycle
- **Multi-threading**: Concurrent handling of multiple client connections
- **Graceful Shutdown**: Signal handling for clean server termination

## Requirements

- **CMake**: Version 3.10 or higher
- **C++ Compiler**: Supporting C++14 standard
  - Windows: Visual Studio 2015+ or MinGW-w64
  - Linux: GCC 5.0+ or Clang 3.4+
- **System Libraries**:
  - Windows: Winsock2 (ws2_32.lib)
  - Linux: pthread, rt

## Installation

### Prerequisites

1. Install CMake from [cmake.org](https://cmake.org/download/)
2. Install a compatible C++ compiler for your platform

### Building the Project

1. **Clone or navigate to the project directory:**
   ```bash
   cd gameserver
   ```

2. **Create and navigate to build directory:**
   ```bash
   mkdir build
   cd build
   ```

3. **Configure with CMake:**
   ```bash
   cmake ..
   ```

4. **Build the project:**
   ```bash
   cmake --build .
   ```

   Or on Linux/macOS:
   ```bash
   make
   ```

## Running the Server

### Start the Server

After building, run the server from the build directory:

```bash
# Windows
./GameServer.exe

# Linux/macOS
./GameServer
```

The server will start on port 8080 by default.

### Server Controls

- **Start**: The server automatically initializes and begins accepting connections
- **Stop**: Press `Ctrl+C` to gracefully shut down the server
- **Port**: Default port is 8080 (configurable in Main.cpp)

### Sample Output

When you start the server, you'll see:
```
Server is running on port 8080... (Press Ctrl+C to stop)
Server running...
```

## Project Structure

```
gameserver/
├── CMakeLists.txt      # Build configuration
├── Main.cpp            # Main entry point and server initialization
├── GameServer.h        # Server class header
├── GameServer.cpp      # Server implementation
├── Room.h              # Room management header
├── Room.cpp            # Room implementation
└── build/              # Build output directory
```

## Development

### Adding New Features

1. **New Game Types**: Extend the Room class to support different game modes
2. **Additional Protocols**: Modify the networking code in GameServer.cpp
3. **Database Integration**: Add persistent storage for rooms and players
4. **Web Interface**: Create a web dashboard for server monitoring

### Building for Different Platforms

The project automatically detects the platform and links appropriate libraries:
- **Windows**: Links ws2_32.lib for networking
- **Linux**: Links pthread and rt libraries
- **macOS**: Uses standard POSIX networking

## Troubleshooting

### Common Issues

1. **Port Already in Use**: Change the port number in Main.cpp (line 58)
2. **Build Errors**: Ensure CMake version is 3.10+ and compiler supports C++14
3. **Network Issues**: Check firewall settings and ensure port 8080 is accessible

### Debug Mode

To build in debug mode, modify CMakeLists.txt:
```cmake
set(CMAKE_BUILD_TYPE Debug)
```

## License

This project is open source. Feel free to modify and distribute according to your needs.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

---

**Note**: This is a basic game server implementation. For production use, consider adding authentication, encryption, logging, and monitoring capabilities.
