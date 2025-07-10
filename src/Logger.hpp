#pragma once

#include <filesystem>
#include <string>
#include <fstream>

enum class LogLevel {
    Debug = 0,
    Info = 1,
    Warn = 2,
    Error = 3,
    Critical = 4
};

class Logger {
    private:
        static std::ofstream logFile;
        static std::filesystem::path logFilePath;
        static LogLevel minLogLevel;
        static bool initialized;
        
        static std::string levelToString(LogLevel level);
        static std::string getCurrentTimestamp();
        static void writeToFile(const std::string& message);

    public:
        static void init(const std::filesystem::path& filePath = "logs/game.log", LogLevel minLevel = LogLevel::Info);
        static void shutdown();
        
        static void log(const std::string& message, LogLevel level = LogLevel::Info);
        static void log(const std::string& prefix, const std::string& message, LogLevel level = LogLevel::Info);

        static void info(const std::string& message);
        static void warn(const std::string& message);
        static void error(const std::string& message);
        static void debug(const std::string& message);
        static void critical(const std::string& message);
        
        static void info(const std::string& prefix, const std::string& message);
        static void warn(const std::string& prefix, const std::string& message);
        static void error(const std::string& prefix, const std::string& message);
        static void debug(const std::string& prefix, const std::string& message);
        static void critical(const std::string& prefix, const std::string& message);
        
        static void setMinLogLevel(LogLevel level);
};