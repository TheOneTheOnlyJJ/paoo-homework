#pragma once

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
    // Static attributes
    
    // Static methods

    // Instance attributes
    bool ansi_codes_enabled = true;
    // Constructors & destructors
    explicit Logger(const string& scope);
    Logger(const string& scope, const Logger::LogLevel log_level);
    Logger(const Logger& other);
    ~Logger();
    // Instance methods
    Logger& operator=(const Logger& other);
    void silly(const string& message) const;
    void verbose(const string& message) const;
    void debug(const string& message) const;
    void info(const string& message) const;
    void warn(const string& message) const;
    void error(const string& message) const;
    Logger::LogLevel getLogLevel() const;
    void setLogLevel(const Logger::LogLevel log_level);
    vector<AnsiCode> getTimestampAnsiCodes() const;
    void setTimestampAnsiCodes(const vector<AnsiCode> ansi_codes);
    vector<AnsiCode> getScopeAnsiCodes() const;
    void setScopeAnsiCodes(const vector<AnsiCode> ansi_codes);
    vector<AnsiCode> getLogLevelAnsiCodes(const Logger::LogLevel log_level) const;
    void setLogLevelAnsiCodes(const Logger::LogLevel log_level, const vector<AnsiCode> ansi_codes);
private:
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
    static const Logger::LogLevel DEFAULT_LOG_LEVEL = Logger::LogLevel::INFO;
    // Static methods
    static string logLevelToString(const Logger::LogLevel log_level);
    // Instance attributes
    Logger::LogLevel log_level;
    char* scope;
    Logger::AnsiCodeMap ansi_code_map;
    // Instance methods
    void log(const Logger::LogLevel log_level, const string& message) const;
};
