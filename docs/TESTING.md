# Testing Guide

This document explains how to run tests for the Game Server project, specifically the Logger utility tests.

## Overview

The project includes comprehensive tests for the Logger utility to ensure it works correctly across different scenarios including:
- Basic logging functionality
- Log level filtering
- File and console output
- Thread safety
- Configuration options
- Log macros
- Edge cases

## Test Files

- **`tests/LoggerTest.cpp`** - Main test suite for the Logger utility
- **`tests/run_tests.bat`** - Windows batch script to run tests
- **`tests/run_tests.sh`** - Linux/Mac shell script to run tests

## Prerequisites

1. **CMake 3.10+** installed
2. **C++17 compatible compiler** (GCC, Clang, or MSVC)
3. **Project built** with `cmake --build .`

## Building Tests

### Step 1: Build the Project

```bash
# From the project root directory
mkdir build
cd build
cmake ..
cmake --build .
```

This will create both the main `GameServer.exe` and the `LoggerTest.exe` in the `build/bin/` directory.

### Step 2: Verify Test Executable

After building, you should see:
```
build/bin/
├── GameServer.exe
└── LoggerTest.exe
```

## Running Tests

### Method 1: Using Test Scripts (Recommended)

#### Windows
```cmd
# From the project root
tests\run_tests.bat
```

#### Linux/Mac
```bash
# From the project root
chmod +x tests/run_tests.sh
tests/run_tests.sh
```

### Method 2: Manual Execution

```bash
# From the project root
cd build/bin
./LoggerTest.exe        # Windows
./LoggerTest            # Linux/Mac
```

### Method 3: From Build Directory

```bash
# From the build directory
cd bin
./LoggerTest.exe        # Windows
./LoggerTest            # Linux/Mac
```

## Test Output

When tests run successfully, you should see output like:

```
Running Logger Tests...
  Testing basic logging... PASSED ✅
  Testing log levels... PASSED ✅
  Testing file output... PASSED ✅
  Testing console output... PASSED ✅
  Testing thread safety... PASSED ✅
  Testing configuration... PASSED ✅
  Testing log macros... PASSED ✅
  Testing edge cases... PASSED ✅
All Logger tests passed! ✅
```

## What the Tests Cover

### 1. **Basic Logging** ✅
- Tests all log levels (DEBUG, INFO, WARN, ERR, FATAL)
- Verifies messages are written to log files
- Checks timestamp formatting

### 2. **Log Levels** ✅
- Tests log level filtering (e.g., INFO level excludes DEBUG messages)
- Verifies only appropriate messages appear based on current level
- Tests level switching at runtime

### 3. **File Output** ✅
- Tests file creation and writing
- Verifies file output can be enabled/disabled
- Checks message persistence in log files

### 4. **Console Output** ✅
- Tests console output configuration
- Verifies console settings can be changed
- Tests output destination switching

### 5. **Thread Safety** ✅
- Tests concurrent logging from multiple threads
- Verifies no data corruption under load
- Tests mutex protection for shared resources

### 6. **Configuration** ✅
- Tests log level setting/getting
- Verifies string-to-level conversion
- Tests level-to-string conversion
- Checks configuration persistence

### 7. **Log Macros** ✅
- Tests all convenience macros (LOG_DEBUG, LOG_INFO, etc.)
- Verifies conditional debug macro (LOG_DEBUG_IF)
- Ensures macros work identically to direct calls

### 8. **Edge Cases** ✅
- Tests empty messages
- Tests very long messages
- Tests special characters
- Tests unicode support

## Test Environment

The tests automatically:
- **Create temporary log files** for testing
- **Restore original logger settings** after tests complete
- **Clean up test files** to avoid clutter
- **Use isolated configuration** to avoid affecting main application

## Troubleshooting

### Common Issues

#### 1. **"LoggerTest.exe not found"**
- **Solution**: Build the project first with `cmake --build .`
- **Check**: Ensure you're in the correct directory

#### 2. **Build Errors**
- **Solution**: Check that all dependencies are installed
- **Check**: Verify CMake version is 3.10+
- **Check**: Ensure C++17 compiler is available

#### 3. **Test Failures**
- **Check**: Look for specific error messages in test output
- **Check**: Verify file permissions for log file creation
- **Check**: Ensure no other processes are using the test log file

#### 4. **Permission Denied (Linux/Mac)**
- **Solution**: Make test script executable: `chmod +x tests/run_tests.sh`

### Debug Mode

To run tests with more verbose output, you can modify the test file to add debug prints or run with a debugger:

```bash
# Windows (using Visual Studio debugger)
cd build/bin
devenv LoggerTest.exe

# Linux/Mac (using GDB)
cd build/bin
gdb ./LoggerTest
```

## Adding New Tests

To add new tests:

1. **Add test method** to the `LoggerTest` class in `tests/LoggerTest.cpp`
2. **Call the new test** in the `runAllTests()` method
3. **Rebuild** the project
4. **Run tests** to verify new functionality

### Example Test Method

```cpp
void testNewFeature() {
    std::cout << "  Testing new feature... ";
    
    // Test implementation here
    auto& logger = Logger::getInstance();
    logger.info("Test message");
    
    // Verification
    // ... verification code ...
    
    std::cout << "PASSED ✅" << std::endl;
}
```

## Continuous Integration

For CI/CD pipelines, you can run tests automatically:

```bash
# Build and test in one command
cmake --build . && ./bin/LoggerTest
```

## Performance Testing

The thread safety test includes basic performance testing:
- **10 threads** writing **100 messages each**
- **1000 total messages** written concurrently
- **90% success rate** required (allows for file I/O buffering)

## Future Enhancements

Planned test improvements:
1. **Memory leak detection** using Valgrind/AddressSanitizer
2. **Performance benchmarks** for different log levels
3. **Stress testing** with larger message volumes
4. **Integration tests** with the main GameServer
5. **Mock objects** for isolated testing

## Conclusion

The test suite provides comprehensive coverage of the Logger utility, ensuring reliability and correctness across different scenarios. Regular testing helps catch regressions and ensures the logging system works as expected in production environments.

