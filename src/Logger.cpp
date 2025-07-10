#include "Logger.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>

std::ofstream Logger::logFile;
std::filesystem::path Logger::logFilePath;
LogLevel Logger::minLogLevel = LogLevel::Info;
bool Logger::initialized = false;

void Logger::init(const std::filesystem::path& filePath, LogLevel minLevel) {

    if (initialized) {
        return;
    }
    
    logFilePath = filePath;
    minLogLevel = minLevel;
    
    if (logFilePath.has_parent_path()) {
        std::filesystem::create_directories(logFilePath.parent_path());
    }
    
    logFile.open(logFilePath, std::ios::out | std::ios::app);
    
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file: " << logFilePath << std::endl;
        return;
    }
    
    initialized = true;
    
    info("Logger initialized with file: " + logFilePath.string());
}

void Logger::shutdown() {
    if (!initialized) {
        return;
    }
    
    if (logFile.is_open()) {
        info("Logger shutting down");
        logFile.close();
    }
    
    initialized = false;
}

void Logger::writeToFile(const std::string& message) {
    if (logFile.is_open()) {
        logFile << message << std::endl;
        logFile.flush();
    }
}

std::string Logger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::Debug: return "DEBUG";
        case LogLevel::Info: return "INFO";
        case LogLevel::Warn: return "WARN";
        case LogLevel::Error: return "ERROR";
        case LogLevel::Critical: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

std::string Logger::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::tm tm_buf;
    std::tm* tm_ptr = nullptr;

    #if defined(_MSC_VER)
        localtime_s(&tm_buf, &time_t);
        tm_ptr = &tm_buf;
    #else
        localtime_r(&time_t, &tm_buf);
        tm_ptr = &tm_buf;
    #endif

    std::stringstream ss;
    ss << std::put_time(tm_ptr, "%Y-%m-%d %H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << ms.count();

    return ss.str();
}

void Logger::log(const std::string& message, LogLevel level) {
    log("", message, level);
}

void Logger::log(const std::string& prefix, const std::string& message, LogLevel level) {
    
    if (!initialized || level < minLogLevel) {
        return;
    }

    std::stringstream ss;
    ss << "[" << getCurrentTimestamp() << "] ";
    
    if (!prefix.empty()) {
        ss << "[" << prefix << "/" << levelToString(level) << "] ";
    } else {
        ss << "[" << levelToString(level) << "] ";
    }
    
    ss << message;
    
    std::string formattedMessage = ss.str();
    
    writeToFile(formattedMessage);
    
    if (level != LogLevel::Debug) {
        std::cout << formattedMessage << std::endl;
    }
}

void Logger::info(const std::string& message) {
    log("", message, LogLevel::Info);
}

void Logger::warn(const std::string& message) {
    log("", message, LogLevel::Warn);
}

void Logger::error(const std::string& message) {
    log("", message, LogLevel::Error);
}

void Logger::debug(const std::string& message) {
    log("", message, LogLevel::Debug);
}

void Logger::critical(const std::string& message) {
    log("", message, LogLevel::Critical);
}

void Logger::info(const std::string& prefix, const std::string& message) {
    log(prefix, message, LogLevel::Info);
}

void Logger::warn(const std::string& prefix, const std::string& message) {
    log(prefix, message, LogLevel::Warn);
}

void Logger::error(const std::string& prefix, const std::string& message) {
    log(prefix, message, LogLevel::Error);
}

void Logger::debug(const std::string& prefix, const std::string& message) {
    log(prefix, message, LogLevel::Debug);
}

void Logger::critical(const std::string& prefix, const std::string& message) {
    log(prefix, message, LogLevel::Critical);
}

void Logger::setMinLogLevel(LogLevel level) {
    minLogLevel = level;
}
