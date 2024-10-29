#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "utils.hpp"

using namespace std;

class Logger
{
public:
    enum class LogLevel: uint8_t
    {
        SILLY = 0,
        VERBOSE,
        DEBUG,
        INFO,
        WARN,
        ERROR
    };
    struct AnsiCodeMap
    {
        vector<AnsiCode> timestamp, scope;
        map<LogLevel, vector<AnsiCode>> level;

        AnsiCodeMap()
            : timestamp({ AnsiCode::BOLD }), scope({ AnsiCode::BOLD })
        {
            level[Logger::LogLevel::SILLY]   = { AnsiCode::BOLD };
            level[Logger::LogLevel::VERBOSE] = { AnsiCode::BOLD };
            level[Logger::LogLevel::DEBUG]   = { AnsiCode::BOLD, AnsiCode::BLUE };
            level[Logger::LogLevel::INFO]    = { AnsiCode::BOLD, AnsiCode::GREEN };
            level[Logger::LogLevel::WARN]    = { AnsiCode::BOLD, AnsiCode::YELLOW };
            level[Logger::LogLevel::ERROR]   = { AnsiCode::BOLD, AnsiCode::RED };
        }
    };
    // Static attributes

    // Static methods

    // Instance attributes
    Logger::AnsiCodeMap ansi_code_map;
    bool use_ansi_codes = true;
    // Constructors & destructors
    explicit Logger(const string& scope);
    Logger(const string& scope, const Logger::LogLevel log_level);
    Logger(const Logger& other);
    ~Logger();
    // Instance methods
    Logger& operator=(const Logger& other);
    Logger::LogLevel getLogLevel() const;
    void setLogLevel(Logger::LogLevel log_level);
    void silly(const string& message) const;
    void verbose(const string& message) const;
    void debug(const string& message) const;
    void info(const string& message) const;
    void warn(const string& message) const;
    void error(const string& message) const;
private:
    // Static attributes
    static const Logger::LogLevel DEFAULT_LOG_LEVEL = Logger::LogLevel::INFO;
    // Static methods
    static string logLevelToString(Logger::LogLevel log_level);
    // Instance attributes
    Logger::LogLevel log_level;
    string scope;
    // Instance methods
    void log(const Logger::LogLevel log_level, const string& message) const;
};

#endif // LOGGER_H
