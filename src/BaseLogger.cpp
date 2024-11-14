#include <cstring>
#include "../inc/BaseLogger.hpp"
#include "../inc/utils.hpp"
#include "BaseLogger.hpp"

using namespace std;

BaseLogger::BaseLogger(const string& scope)
    : scope((char *) malloc((scope.length() + 1) * sizeof(char))), log_level(BaseLogger::DEFAULT_LOG_LEVEL)
{
    if (!this->scope) {
        exit(EXIT_FAILURE);
    }
    strcpy(this->scope, scope.c_str());
    cout << "Initialised BaseLogger with scope: " << this->scope << " and default log level: " << BaseLogger::logLevelToString(this->log_level) << "." << endl;
}

BaseLogger::BaseLogger(const string& scope, const BaseLogger::LogLevel log_level)
    : scope((char *) malloc((scope.length() + 1) * sizeof(char))), log_level(log_level)
{
    if (!this->scope) {
        exit(EXIT_FAILURE);
    }
    strcpy(this->scope, scope.c_str());
    cout << "Initialised BaseLogger with scope: " << this->scope << " and log level: " << BaseLogger::logLevelToString(this->log_level) << "." << endl;
}

BaseLogger::BaseLogger(const BaseLogger& other) 
    : scope((char *) malloc((strlen(other.scope) + 1) * sizeof(char))), log_level(other.log_level)
{
    if (!this->scope) {
        exit(EXIT_FAILURE);
    }
    strcpy(this->scope, other.scope);
    cout << "Initialised BaseLogger with scope: " << this->scope << " and log level: " << BaseLogger::logLevelToString(this->log_level) << " using copy constructor." << endl;
}

BaseLogger::BaseLogger(BaseLogger&& other)
    : scope(other.scope), log_level(other.log_level)
{
    other.scope = nullptr;
    cout << "Initialised BaseLogger with scope: " << this->scope << " and log level: " << BaseLogger::logLevelToString(this->log_level) << " using move constructor." << endl;
}

BaseLogger::~BaseLogger()
{
    cout << "Destroying BaseLogger with scope: " << (this->scope ? this->scope : "nullptr") << "." << endl;
    free(this->scope);
}

void BaseLogger::log(const BaseLogger::LogLevel log_level, const string& message) const
{
    cout << "Base log method implementation. No-op." << endl;
}

BaseLogger& BaseLogger::operator=(const BaseLogger& other)
{
    cout << "Assigning BaseLogger with scope " << other.scope << " to BaseLogger with scope " << this->scope << "." << endl;
    if (this != &other) {
        this->scope = (char *) realloc(this->scope, (strlen(other.scope) + 1) * sizeof(char));
        if (!this->scope) {
            exit(EXIT_FAILURE);
        }
        strcpy(this->scope, other.scope);
        this->log_level = other.log_level;
    } else {
        cout << "Self-assignment detected. No-op." << endl;
    }
    return *this;
}

BaseLogger& BaseLogger::operator=(BaseLogger&& other)
{
    if (this != &other)
    {
        if (scope) {
            free(scope);
        }
        scope = other.scope;
        log_level = other.log_level;
        other.scope = nullptr;
    } else {
        cout << "Self-assignment detected. No-op." << endl;
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

void BaseLogger::silly(const string& message) const
{
    this->log(BaseLogger::LogLevel::SILLY, message);
}

void BaseLogger::verbose(const string& message) const
{
    this->log(BaseLogger::LogLevel::VERBOSE, message);
}

void BaseLogger::debug(const string& message) const
{
    this->log(BaseLogger::LogLevel::DEBUG, message);
}

void BaseLogger::info(const string& message) const
{
    this->log(BaseLogger::LogLevel::INFO, message);
}

void BaseLogger::warn(const string& message) const
{
    this->log(BaseLogger::LogLevel::WARN, message);
}

void BaseLogger::error(const string& message) const
{
    this->log(BaseLogger::LogLevel::ERROR, message);
}

BaseLogger::LogLevel BaseLogger::getLogLevel() const
{
    cout << "Getting log level." << endl;
    return this->log_level;
}

void BaseLogger::setLogLevel(const BaseLogger::LogLevel log_level)
{
    cout << "Setting log level to: " << BaseLogger::logLevelToString(log_level) << "." << endl;
    this->log_level = log_level;
}
