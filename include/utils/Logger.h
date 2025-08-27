#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>
#include <memory>
#include <sstream>

class Logger {
public:
    enum class Level {
        DEBUG = 0,
        INFO = 1,
        WARN = 2,
        ERR = 3,
        FATAL = 4
    };

    // Singleton pattern for global logger instance
    static Logger& getInstance();
    
    // Configuration methods
    void setLogFile(const std::string& filename);
    void setLogLevel(Level level);
    void setConsoleOutput(bool enable);
    void setFileOutput(bool enable);
    
    // Logging methods
    void debug(const std::string& message);
    void info(const std::string& message);
    void warn(const std::string& message);
    void err(const std::string& message);
    void fatal(const std::string& message);
    
    // Generic log method
    void log(Level level, const std::string& message);
    
    // Utility methods
    std::string levelToString(Level level);
    Level stringToLevel(const std::string& levelStr);
    
    // Destructor
    ~Logger();

private:
    Logger(); // Private constructor for singleton
    Logger(const Logger&) = delete; // Disable copy constructor
    Logger& operator=(const Logger&) = delete; // Disable assignment operator
    
    void writeToFile(const std::string& message);
    void writeToConsole(const std::string& message);
    std::string getTimestamp();
    std::string formatMessage(Level level, const std::string& message);
    
    // Member variables
    std::ofstream logFile;
    std::string logFilename;
    Level currentLevel;
    bool consoleOutput;
    bool fileOutput;
    std::mutex logMutex;
    
    // Static constants
    static const std::string DEFAULT_LOG_FILE;
    static const Level DEFAULT_LEVEL;
};

// Convenience macros for easier logging
#define LOG_DEBUG(msg) Logger::getInstance().debug(msg)
#define LOG_INFO(msg)  Logger::getInstance().info(msg)
#define LOG_WARN(msg)  Logger::getInstance().warn(msg)
#define LOG_ERR(msg) Logger::getInstance().err(msg)
#define LOG_FATAL(msg) Logger::getInstance().fatal(msg)

// Macro for conditional debug logging
#ifdef _DEBUG
    #define LOG_DEBUG_IF(condition, msg) if(condition) Logger::getInstance().debug(msg)
#else
    #define LOG_DEBUG_IF(condition, msg)
#endif

#endif // LOGGER_H
