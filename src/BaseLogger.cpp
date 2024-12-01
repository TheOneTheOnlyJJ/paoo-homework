#include <cstring>
#include <memory>
#include <iostream>
#include <fstream>
#include <mutex>
#include "../inc/BaseLogger.hpp"
#include "../inc/utils.hpp"
#include "BaseLogger.hpp"

using namespace std;

BaseLogger::BaseLogger(const string &scope, const shared_ptr<BaseLogger::LifecycleLogResources> &lifecycle_log_resources)
    : scope((char *) malloc((scope.length() + 1) * sizeof(char))), log_level(BaseLogger::DEFAULT_LOG_LEVEL), lifecycle_log_resources(lifecycle_log_resources)
{
    if (!this->scope)
    {
        exit(EXIT_FAILURE);
    }
    strcpy(this->scope, scope.c_str());
    this->logLifecycleMessage("[BaseLogger] [" + string(this->scope) + "] [" + BaseLogger::logLevelToString(this->log_level) + " (default)]: Initialised.");
}

BaseLogger::BaseLogger(const string &scope, const BaseLogger::LogLevel log_level, const shared_ptr<BaseLogger::LifecycleLogResources> &lifecycle_log_resources)
    : scope((char *) malloc((scope.length() + 1) * sizeof(char))), log_level(log_level), lifecycle_log_resources(lifecycle_log_resources)
{
    if (!this->scope)
    {
        exit(EXIT_FAILURE);
    }
    strcpy(this->scope, scope.c_str());
    this->logLifecycleMessage("[BaseLogger] [" + string(this->scope) + "] [" + BaseLogger::logLevelToString(this->log_level) + "]: Initialised.");
}

BaseLogger::BaseLogger(const BaseLogger &other) 
    : scope((char *) malloc((strlen(other.scope) + 1) * sizeof(char))), log_level(other.log_level), lifecycle_log_resources(other.lifecycle_log_resources)
{
    if (!this->scope)
    {
        exit(EXIT_FAILURE);
    }
    strcpy(this->scope, other.scope);
    this->logLifecycleMessage("[BaseLogger] [" + string(this->scope) + "] [" + BaseLogger::logLevelToString(this->log_level) + "]: Initialised with copy constructor.");
}

BaseLogger::BaseLogger(BaseLogger &&other)
    : scope(other.scope), log_level(other.log_level), lifecycle_log_resources(other.lifecycle_log_resources)
{
    other.scope = nullptr;
    this->logLifecycleMessage("[BaseLogger] [" + string(this->scope) + "] [" + BaseLogger::logLevelToString(this->log_level) + "]: Initialised with move constructor.");
}

BaseLogger::~BaseLogger()
{
    if (!this->scope)
    {
        this->logLifecycleMessage("[BaseLogger]: Destroying moved-from instance.");
    }
    else
    {
        this->logLifecycleMessage("[BaseLogger] [" + string(this->scope) + "]: Destroying instance.");
        free(this->scope);
    }
}

void BaseLogger::log(const BaseLogger::LogLevel log_level, const string &message) const
{
    throw new runtime_error("[BaseLogger]: Log method not implemented!");
}

void BaseLogger::logLifecycleMessage(const string &message) const
{
    lock_guard<mutex> lock(*this->lifecycle_log_resources->file_mutex);
    if (this->lifecycle_log_resources->file_stream && this->lifecycle_log_resources->file_stream->is_open()) {
        *this->lifecycle_log_resources->file_stream << "[" << getCurrentDateTime() << "] " << message << endl;
    }
}

BaseLogger &BaseLogger::operator=(const BaseLogger &other)
{
    this->logLifecycleMessage("[BaseLogger] [" + string(other.scope) + "]: Assigning to [BaseLogger] [" + string(this->scope) + "] with assignment operator.");
    if (this != &other)
    {
        this->scope = (char *) realloc(this->scope, (strlen(other.scope) + 1) * sizeof(char));
        if (!this->scope)
        {
            exit(EXIT_FAILURE);
        }
        strcpy(this->scope, other.scope);
        this->log_level = other.log_level;
        this->lifecycle_log_resources = other.lifecycle_log_resources;
    }
    else
    {
        this->logLifecycleMessage("[BaseLogger] [" + string(other.scope) + "]: Self assignment detected. No-op.");
    }
    return *this;
}

BaseLogger &BaseLogger::operator=(BaseLogger &&other)
{
    this->logLifecycleMessage("[BaseLogger] [" + string(other.scope) + "]: Assigning to [BaseLogger] [" + string(this->scope) + "] with move assignment operator.");
    if (this != &other)
    {
        if (this->scope)
        {
            free(this->scope);
        }
        this->scope = other.scope;
        this->log_level = other.log_level;
        this->lifecycle_log_resources = other.lifecycle_log_resources;
        other.scope = nullptr;
    }
    else
    {
        this->logLifecycleMessage("[BaseLogger] [" + string(other.scope) + "]: Self move assignment detected. No-op.");
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
    this->logLifecycleMessage("[BaseLogger] [" + string(this->scope) + "]: Getting log level (" + string(this->scope) + ").");
    return this->log_level;
}

void BaseLogger::setLogLevel(const BaseLogger::LogLevel log_level)
{
    this->logLifecycleMessage("[BaseLogger] [" + string(this->scope) + "]: Setting log level to " + BaseLogger::logLevelToString(log_level) + ".");
    this->log_level = log_level;
}
