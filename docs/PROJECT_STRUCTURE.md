# Project Structure

This document describes the organized structure of the Game Server project.

## Directory Layout

```
gameserver/
├── src/                    # Source code files
│   ├── core/              # Core server functionality
│   │   └── GameServer.cpp # Main server implementation
│   ├── game/              # Game-specific logic
│   │   └── Room.cpp       # Room management implementation
│   └── Main.cpp           # Application entry point
├── include/                # Header files
│   ├── core/              # Core server headers
│   │   └── GameServer.h   # Main server class header
│   └── game/              # Game-specific headers
│       └── Room.h         # Room management header
├── build/                  # Build output (generated)
├── docs/                   # Documentation
├── examples/               # Example code and usage
├── tests/                  # Unit tests and test files
├── CMakeLists.txt          # Build configuration
└── README.md               # Project overview
```

## Module Organization

### Core Module (`src/core/`, `include/core/`)
- **GameServer**: Main server class handling networking, room management, and client connections
- **Network handling**: Socket management, client communication
- **Server lifecycle**: Initialization, shutdown, and main loop

### Game Module (`src/game/`, `include/game/`)
- **Room**: Game room management with player handling
- **Game logic**: Game state management and rules
- **Player management**: Player states and interactions

### Main Entry Point (`src/Main.cpp`)
- **Application startup**: Server initialization and configuration
- **Signal handling**: Graceful shutdown management
- **Main loop**: Server runtime execution

## Benefits of This Structure

1. **Separation of Concerns**: Each module has a specific responsibility
2. **Maintainability**: Easy to locate and modify specific functionality
3. **Scalability**: Simple to add new modules and features
4. **Testing**: Isolated modules are easier to unit test
5. **Documentation**: Clear organization makes code easier to understand

## Adding New Features

### New Game Types
- Add headers to `include/game/`
- Add implementations to `src/game/`
- Update `CMakeLists.txt` with new source files

### New Core Features
- Add headers to `include/core/`
- Add implementations to `src/core/`
- Update `CMakeLists.txt` with new source files

### Utility Functions
- Use `src/utils/` and `include/utils/` for common helper functions
- Keep utilities generic and reusable across modules

## Build Process

The new structure maintains the same build process:
1. Create build directory: `mkdir build && cd build`
2. Configure: `cmake ..`
3. Build: `cmake --build .`

The executable will be placed in `build/bin/` for better organization.
