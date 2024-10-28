#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>

using namespace std;

class Logger {
public:
    enum class LogLevels: int
        {
            SILLY = 0,
            VERBOSE,
            DEBUG,
            INFO,
            WARN,
            ERROR
        };
    explicit Logger(const string& scope);
    explicit Logger(const string& scope, const Logger::LogLevels logLevel);
    ~Logger();
    Logger::LogLevels getLogLevel();
    void setLogLevel(Logger::LogLevels logLevel);
    void silly(const string& message) const;
    void verbose(const string& message) const;
    void debug(const string& message) const;
    void info(const string& message) const;
    void warn(const string& message) const;
    void error(const string& message) const;
private:
    static string logLevelToString(Logger::LogLevels logLevel);
    static const Logger::LogLevels DEFAULT_LOG_LEVEL = Logger::LogLevels::INFO; 
    Logger::LogLevels logLevel;
    string scope;
    void log(const Logger::LogLevels logLevel, const string& message) const;
};

#endif // LOGGER_H
