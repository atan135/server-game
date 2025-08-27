#include "utils/Logger.h"
#include <iostream>

// Example of how to use the Logger utility
int main() {
    // Configure logger
    Logger::getInstance().setLogFile("logs/example.log");
    Logger::getInstance().setLogLevel(Logger::Level::DEBUG);
    Logger::getInstance().setConsoleOutput(true);
    Logger::getInstance().setFileOutput(true);
    
    // Different log levels
    LOG_DEBUG("This is a debug message");
    LOG_INFO("This is an info message");
    LOG_WARN("This is a warning message");
    LOG_ERR("This is an error message");
    LOG_FATAL("This is a fatal message");
    
    // Using convenience macros
    LOG_DEBUG("Debug message using macro");
    LOG_INFO("Info message using macro");
    LOG_WARN("Warning message using macro");
    LOG_ERR("Error message using macro");
    LOG_FATAL("Fatal message using macro");
    
    // Conditional debug logging
    LOG_DEBUG_IF(true, "This debug message will be shown");
    LOG_DEBUG_IF(false, "This debug message will NOT be shown");
    
    // Example with variables
    int playerCount = 5;
    std::string playerName = "Alice";
    LOG_INFO("Player " + playerName + " joined. Total players: " + std::to_string(playerCount));
    
    // Change log level at runtime
    Logger::getInstance().setLogLevel(Logger::Level::WARN);
    LOG_DEBUG("This debug message won't be shown now");
    LOG_WARN("This warning will still be shown");
    
    return 0;
}
