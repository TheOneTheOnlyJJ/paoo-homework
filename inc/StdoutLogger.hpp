#pragma once

#include <vector>
#include <map>
#include "utils.hpp"
#include "BaseLogger.hpp"

using namespace std;

class StdoutLogger : public BaseLogger
{
public:
    // Static attributes
    // Static methods
    // Instance attributes
    bool ansi_codes_enabled = true;
    // Constructors & destructors
    explicit StdoutLogger(const string& scope);
    StdoutLogger(const string& scope, const BaseLogger::LogLevel log_level);
    StdoutLogger(const StdoutLogger& other);
    ~StdoutLogger();
    // Instance methods
    StdoutLogger& operator=(const StdoutLogger& other);
    vector<AnsiCode> getTimestampAnsiCodes() const;
    void setTimestampAnsiCodes(const vector<AnsiCode> ansi_codes);
    vector<AnsiCode> getScopeAnsiCodes() const;
    void setScopeAnsiCodes(const vector<AnsiCode> ansi_codes);
    vector<AnsiCode> getLogLevelAnsiCodes(const BaseLogger::LogLevel log_level) const;
    void setLogLevelAnsiCodes(const BaseLogger::LogLevel log_level, const vector<AnsiCode> ansi_codes);
protected:
    // Static attributes
    // Static methods
    // Instance attributes
    // Instance methods
    void log(const BaseLogger::LogLevel log_level, const string &message) const override;
private:
    struct AnsiCodeMap
    {
        vector<AnsiCode> timestamp, scope;
        map<BaseLogger::LogLevel, vector<AnsiCode>> level;

        AnsiCodeMap()
            : timestamp({ AnsiCode::BOLD }), scope({ AnsiCode::BOLD })
        {
            level[BaseLogger::LogLevel::SILLY]   = { AnsiCode::BOLD };
            level[BaseLogger::LogLevel::VERBOSE] = { AnsiCode::BOLD };
            level[BaseLogger::LogLevel::DEBUG]   = { AnsiCode::BOLD, AnsiCode::BLUE };
            level[BaseLogger::LogLevel::INFO]    = { AnsiCode::BOLD, AnsiCode::GREEN };
            level[BaseLogger::LogLevel::WARN]    = { AnsiCode::BOLD, AnsiCode::YELLOW };
            level[BaseLogger::LogLevel::ERROR]   = { AnsiCode::BOLD, AnsiCode::RED };
        }
    };
    // Static attributes
    // Static methods
    // Instance attributes
    StdoutLogger::AnsiCodeMap ansi_code_map;
    // Instance methods
};
