#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include <fstream>
#include <mutex>
#include "utils.hpp"

using namespace std;

class BaseLogger
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
    static const BaseLogger::LogLevel DEFAULT_LOG_LEVEL = BaseLogger::LogLevel::INFO;
    // Static methods
    // Instance attributes
    // Constructors & destructors
    BaseLogger() = delete;
    explicit BaseLogger(const string &scope, const shared_ptr<ofstream> &lifecycle_log_file_stream, const shared_ptr<mutex> &lifecycle_log_mutex);
    BaseLogger(const string &scope, const BaseLogger::LogLevel log_level, const shared_ptr<ofstream> &lifecycle_log_file_stream, const shared_ptr<mutex> &lifecycle_log_mutex);
    BaseLogger(const BaseLogger &other);
    BaseLogger(BaseLogger &&other);
    virtual ~BaseLogger();
    // Instance methods
    BaseLogger &operator=(const BaseLogger &other);
    BaseLogger &operator=(BaseLogger &&other);
    void silly(const string &message) const;
    void verbose(const string &message) const;
    void debug(const string &message) const;
    void info(const string &message) const;
    void warn(const string &message) const;
    void error(const string &message) const;
    BaseLogger::LogLevel getLogLevel() const;
    void setLogLevel(const BaseLogger::LogLevel log_level);
protected:
    // Static attributes
    // Static methods
    static string logLevelToString(const BaseLogger::LogLevel log_level);
    // Instance attributes
    BaseLogger::LogLevel log_level;
    char *scope;
    // Instance methods
    virtual void log(const BaseLogger::LogLevel log_level, const string &message) const;
    void logLifecycleMessage(const string &message) const;
private:
    // Static attributes
    // Static methods
    // Instance attributes
    shared_ptr<ofstream> lifecycle_log_file_stream;
    shared_ptr<mutex> lifecycle_log_mutex;
    // Instance methods
};
