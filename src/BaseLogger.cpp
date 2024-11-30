#include <cstring>
#include <memory>
#include <iostream>
#include <fstream>
#include <mutex>
#include "../inc/BaseLogger.hpp"
#include "../inc/utils.hpp"
#include "BaseLogger.hpp"

using namespace std;

BaseLogger::BaseLogger(const string &scope, const shared_ptr<ofstream> &lifecycle_log_file_stream, const shared_ptr<mutex> &lifecycle_log_mutex)
    : scope((char *) malloc((scope.length() + 1) * sizeof(char))), log_level(BaseLogger::DEFAULT_LOG_LEVEL), lifecycle_log_file_stream(lifecycle_log_file_stream), lifecycle_log_mutex(lifecycle_log_mutex)
{
    if (!this->scope)
    {
        exit(EXIT_FAILURE);
    }
    strcpy(this->scope, scope.c_str());
    this->logLifecycleMessage("Initialised BaseLogger with scope: " + string(this->scope) + " and default log level: " + BaseLogger::logLevelToString(this->log_level) + ".");
}

BaseLogger::BaseLogger(const string &scope, const BaseLogger::LogLevel log_level, const shared_ptr<ofstream> &lifecycle_log_file_stream, const shared_ptr<mutex> &lifecycle_log_mutex)
    : scope((char *) malloc((scope.length() + 1) * sizeof(char))), log_level(log_level), lifecycle_log_file_stream(lifecycle_log_file_stream), lifecycle_log_mutex(lifecycle_log_mutex)
{
    if (!this->scope)
    {
        exit(EXIT_FAILURE);
    }
    strcpy(this->scope, scope.c_str());
    this->logLifecycleMessage("Initialised BaseLogger with scope: " + string(this->scope) + " and log level: " + BaseLogger::logLevelToString(this->log_level) + ".");
}

BaseLogger::BaseLogger(const BaseLogger &other) 
    : scope((char *) malloc((strlen(other.scope) + 1) * sizeof(char))), log_level(other.log_level), lifecycle_log_file_stream(other.lifecycle_log_file_stream), lifecycle_log_mutex(other.lifecycle_log_mutex)
{
    if (!this->scope)
    {
        exit(EXIT_FAILURE);
    }
    strcpy(this->scope, other.scope);
    this->logLifecycleMessage("Initialised BaseLogger with scope: " + string(this->scope) + " and log level: " + BaseLogger::logLevelToString(this->log_level) + " using copy constructor.");
}

BaseLogger::BaseLogger(BaseLogger &&other)
    : scope(other.scope), log_level(other.log_level), lifecycle_log_file_stream(other.lifecycle_log_file_stream), lifecycle_log_mutex(other.lifecycle_log_mutex)
{
    other.scope = nullptr;
    this->logLifecycleMessage("Initialised BaseLogger with scope: " + string(this->scope) + " and log level: " + BaseLogger::logLevelToString(this->log_level) + " using move constructor.");
}

BaseLogger::~BaseLogger()
{
    if (!this->scope)
    {
        this->logLifecycleMessage("Destroying moved-from BaseLogger.");
    }
    else
    {
        this->logLifecycleMessage("Destroying BaseLogger with scope: " + string(this->scope) + ".");
        free(this->scope);
    }
}

void BaseLogger::log(const BaseLogger::LogLevel log_level, const string &message) const
{
    this->logLifecycleMessage("Base log method implementation. No-op.");
}

void BaseLogger::logLifecycleMessage(const string &message) const
{
    lock_guard<mutex> lock(*this->lifecycle_log_mutex);
    if (this->lifecycle_log_file_stream && this->lifecycle_log_file_stream->is_open()) {
        *this->lifecycle_log_file_stream << message << endl;
    }
}

BaseLogger &BaseLogger::operator=(const BaseLogger &other)
{
    this->logLifecycleMessage("Assigning BaseLogger with scope " + string(other.scope) + " to BaseLogger with scope " + string(this->scope) + ".");
    if (this != &other)
    {
        this->scope = (char *) realloc(this->scope, (strlen(other.scope) + 1) * sizeof(char));
        if (!this->scope)
        {
            exit(EXIT_FAILURE);
        }
        strcpy(this->scope, other.scope);
        this->log_level = other.log_level;
        this->lifecycle_log_file_stream = other.lifecycle_log_file_stream;
        this->lifecycle_log_mutex = other.lifecycle_log_mutex;
    }
    else
    {
        this->logLifecycleMessage("Self-assignment detected. No-op.");
    }
    return *this;
}

BaseLogger &BaseLogger::operator=(BaseLogger &&other)
{
    this->logLifecycleMessage("Assigning BaseLogger with scope " + string(other.scope) + " to BaseLogger with scope " + string(this->scope) + " using move assignment operator.");
    if (this != &other)
    {
        if (this->scope)
        {
            free(this->scope);
        }
        this->scope = other.scope;
        this->log_level = other.log_level;
        this->lifecycle_log_file_stream = other.lifecycle_log_file_stream;
        this->lifecycle_log_mutex = other.lifecycle_log_mutex;
        other.scope = nullptr;
    }
    else
    {
        this->logLifecycleMessage("Self-assignment detected. No-op.");
    }
    return *this;
}

string BaseLogger::logLevelToString(const BaseLogger::LogLevel log_level)
{
    switch (log_level)
    {
        case BaseLogger::LogLevel::SILLY:
            return "SILLY";
        case BaseLogger::LogLevel::VERBOSE:
            return "VERBOSE";
        case BaseLogger::LogLevel::DEBUG:
            return "DEBUG";
        case BaseLogger::LogLevel::INFO:
            return "INFO";
        case BaseLogger::LogLevel::WARN:
            return "WARN";
        case BaseLogger::LogLevel::ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

void BaseLogger::silly(const string &message) const
{
    this->log(BaseLogger::LogLevel::SILLY, message);
}

void BaseLogger::verbose(const string &message) const
{
    this->log(BaseLogger::LogLevel::VERBOSE, message);
}

void BaseLogger::debug(const string &message) const
{
    this->log(BaseLogger::LogLevel::DEBUG, message);
}

void BaseLogger::info(const string &message) const
{
    this->log(BaseLogger::LogLevel::INFO, message);
}

void BaseLogger::warn(const string &message) const
{
    this->log(BaseLogger::LogLevel::WARN, message);
}

void BaseLogger::error(const string &message) const
{
    this->log(BaseLogger::LogLevel::ERROR, message);
}

BaseLogger::LogLevel BaseLogger::getLogLevel() const
{
    this->logLifecycleMessage("Getting log level.");
    return this->log_level;
}

void BaseLogger::setLogLevel(const BaseLogger::LogLevel log_level)
{
    this->logLifecycleMessage("Setting log level to: " + BaseLogger::logLevelToString(log_level) + ".");
    this->log_level = log_level;
}
