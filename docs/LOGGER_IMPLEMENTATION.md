# Logger Implementation

This document describes the Logger utility that has been integrated into the Game Server project.

## Overview

The Logger utility provides a comprehensive logging system that replaces all `std::cout` and `std::cerr` statements in the project with structured, configurable logging.

## Features

- **Multiple Log Levels**: DEBUG, INFO, WARN, ERR, FATAL
- **File and Console Output**: Configurable output destinations
- **Thread-Safe**: Safe for multi-threaded applications
- **Timestamped**: Each log entry includes precise timestamps
- **Configurable**: Runtime configuration of log levels and output
- **Convenience Macros**: Easy-to-use logging macros

## Files Created

- `include/utils/Logger.h` - Logger header file
- `src/utils/Logger.cpp` - Logger implementation
- `examples/logger_example.cpp` - Usage examples
- `config/logger.conf` - Configuration file template

## Usage

### Basic Logging

```cpp
#include "utils/Logger.h"

// Configure logger
Logger::getInstance().setLogFile("logs/gameserver.log");
Logger::getInstance().setLogLevel(Logger::Level::DEBUG);

// Log messages using macros (recommended)
LOG_DEBUG("Debug message");
LOG_INFO("Info message");
LOG_WARN("Warning message");
LOG_ERR("Error message");
LOG_FATAL("Fatal message");

// Or using direct calls (alternative)
Logger::getInstance().debug("Debug message");
Logger::getInstance().info("Info message");
Logger::getInstance().warn("Warning message");
Logger::getInstance().err("Error message");
Logger::getInstance().fatal("Fatal message");
```

### Using Macros

```cpp
// Convenience macros
LOG_DEBUG("Debug message");
LOG_INFO("Info message");
LOG_WARN("Warning message");
LOG_ERR("Error message");
LOG_FATAL("Fatal message");

// Conditional debug logging
LOG_DEBUG_IF(condition, "Debug message");
```

### Configuration

```cpp
// Set log level
Logger::getInstance().setLogLevel(Logger::Level::WARN);

// Enable/disable outputs
Logger::getInstance().setConsoleOutput(true);
Logger::getInstance().setFileOutput(true);

// Change log file
Logger::getInstance().setLogFile("logs/newfile.log");
```

## Log Levels

1. **DEBUG** (0) - Detailed debugging information
2. **INFO** (1) - General information messages
3. **WARN** (2) - Warning messages
4. **ERR** (3) - Error messages
5. **FATAL** (4) - Fatal error messages

Only messages at or above the current log level are displayed.

## Output Format

Each log message follows this format:
```
[2025-08-27 14:06:30.123] [INFO ] Server listening on port 8080
```

- Timestamp with milliseconds
- Log level (padded to 5 characters)
- Message content

## Integration Changes Made

### Files Modified

1. **src/core/GameServer.cpp**
   - Replaced all `std::cout` with `LOG_INFO()`
   - Replaced all `std::cerr` with `LOG_ERR()`
   - Added debug logging for received data using `LOG_DEBUG()`

2. **src/game/Room.cpp**
   - Replaced `std::cout` with `LOG_INFO()`

3. **src/Main.cpp**
   - Replaced all `std::cout` with appropriate log macros
   - Replaced `std::cerr` with `LOG_ERR()`

4. **CMakeLists.txt**
   - Added Logger utility to build system

### Benefits of the Changes

1. **Structured Logging**: All output is now properly formatted and categorized
2. **File Output**: Logs are saved to files for debugging and monitoring
3. **Log Levels**: Easy to filter messages by importance
4. **Thread Safety**: Safe for concurrent access
5. **Professional Output**: Clean, timestamped log entries
6. **Configurable**: Easy to adjust logging behavior at runtime

## Configuration File

The `config/logger.conf` file provides a template for logger configuration:

```ini
log_level=INFO
log_file=logs/gameserver.log
console_output=true
file_output=true
max_file_size=10MB
log_rotation=true
```

## Future Enhancements

1. **Log Rotation**: Automatic log file rotation based on size/time
2. **Remote Logging**: Send logs to remote servers
3. **Performance Metrics**: Track logging performance impact
4. **Custom Formatters**: Allow custom log message formats
5. **Async Logging**: Non-blocking log writes

## Troubleshooting

### Common Issues

1. **Build Errors**: Ensure `src/utils/Logger.cpp` is included in CMakeLists.txt
2. **Include Paths**: Use `#include "utils/Logger.h"` in source files
3. **Windows Conflicts**: The `ERROR` enum value was renamed to `ERR` to avoid Windows macro conflicts

### Debug Tips

1. Set log level to DEBUG for maximum information
2. Check log files in the `logs/` directory
3. Use `LOG_DEBUG_IF()` for conditional debug output
4. Monitor log file size and rotation

## Example Output

When running the server, you'll now see:

```
[2025-08-27 14:06:30.123] [INFO ] Server listening on port 8080
[2025-08-27 14:06:30.124] [INFO ] === Room List ===
[2025-08-27 14:06:30.125] [INFO ] Room 1 (Battle Room): 2/4 players
[2025-08-27 14:06:30.126] [INFO ] Room 2 (Casual Game): 1/2 players

## Code Style

The project now uses log macros throughout for cleaner, more readable code:

```cpp
// Before (verbose)
Logger::getInstance().info("Server started on port " + std::to_string(port));

// After (clean and readable)
LOG_INFO("Server started on port " + std::to_string(port));
```
[2025-08-27 14:06:30.127] [INFO ] Server is running on port 8080... (Press Ctrl+C to stop)
```

This provides much better visibility into server operation and makes debugging significantly easier.
