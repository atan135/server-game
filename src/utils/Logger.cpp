#include "utils/Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <algorithm>

// Static constants
const std::string Logger::DEFAULT_LOG_FILE = "gameserver.log";
const Logger::Level Logger::DEFAULT_LEVEL = Logger::Level::INFO;

Logger::Logger() 
    : currentLevel(DEFAULT_LEVEL)
    , consoleOutput(true)
    , fileOutput(true)
    , logFilename(DEFAULT_LOG_FILE) {
    
    // Open log file
    if (fileOutput) {
        logFile.open(logFilename, std::ios::app);
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::setLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(logMutex);
    
    if (logFile.is_open()) {
        logFile.close();
    }
    
    logFilename = filename;
    if (fileOutput) {
        logFile.open(logFilename, std::ios::app);
    }
}

void Logger::setLogLevel(Level level) {
    std::lock_guard<std::mutex> lock(logMutex);
    currentLevel = level;
}

void Logger::setConsoleOutput(bool enable) {
    std::lock_guard<std::mutex> lock(logMutex);
    consoleOutput = enable;
}

void Logger::setFileOutput(bool enable) {
    std::lock_guard<std::mutex> lock(logMutex);
    
    if (enable && !fileOutput) {
        logFile.open(logFilename, std::ios::app);
    } else if (!enable && fileOutput) {
        logFile.close();
    }
    
    fileOutput = enable;
}

void Logger::debug(const std::string& message) {
    log(Level::DEBUG, message);
}

void Logger::info(const std::string& message) {
    log(Level::INFO, message);
}

void Logger::warn(const std::string& message) {
    log(Level::WARN, message);
}

void Logger::err(const std::string& message) {
    log(Level::ERR, message);
}

void Logger::fatal(const std::string& message) {
    log(Level::FATAL, message);
}

void Logger::log(Level level, const std::string& message) {
    // Check if we should log this level
    if (level < currentLevel) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(logMutex);
    
    std::string formattedMessage = formatMessage(level, message);
    
    if (consoleOutput) {
        writeToConsole(formattedMessage);
    }
    
    if (fileOutput && logFile.is_open()) {
        writeToFile(formattedMessage);
    }
}

std::string Logger::levelToString(Level level) {
    switch (level) {
        case Level::DEBUG: return "DEBUG";
        case Level::INFO:  return "INFO ";
        case Level::WARN:  return "WARN ";
        case Level::ERR: return "ERR  ";
        case Level::FATAL: return "FATAL";
        default:           return "UNKNOWN";
    }
}

Logger::Level Logger::stringToLevel(const std::string& levelStr) {
    std::string upperLevel = levelStr;
    std::transform(upperLevel.begin(), upperLevel.end(), upperLevel.begin(), ::toupper);
    
    if (upperLevel == "DEBUG") return Level::DEBUG;
    if (upperLevel == "INFO")  return Level::INFO;
    if (upperLevel == "WARN")  return Level::WARN;
    if (upperLevel == "ERR") return Level::ERR;
    if (upperLevel == "FATAL") return Level::FATAL;
    
    return DEFAULT_LEVEL;
}

void Logger::writeToFile(const std::string& message) {
    if (logFile.is_open()) {
        logFile << message << std::endl;
        logFile.flush(); // Ensure immediate write
    }
}

void Logger::writeToConsole(const std::string& message) {
    std::cout << message << std::endl;
    std::cout.flush(); // Ensure immediate output
}

std::string Logger::getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    
    return ss.str();
}

std::string Logger::formatMessage(Level level, const std::string& message) {
    std::stringstream ss;
    ss << "[" << getTimestamp() << "] "
       << "[" << levelToString(level) << "] "
       << message;
    
    return ss.str();
}
