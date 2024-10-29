#include "../inc/Logger.hpp"
#include "../inc/utils.hpp"

using namespace std;

Logger::Logger(const string& scope)
    : scope(scope), logLevel(Logger::DEFAULT_LOG_LEVEL) {
    cout << "Initialising logger with scope: " << this->scope << " and default log level: " << Logger::logLevelToString(this->logLevel) << "." << endl;
}

Logger::Logger(const string& scope, const Logger::LogLevels logLevel)
    : scope(scope), logLevel(logLevel) {
    cout << "Initialising logger with scope: " << this->scope << " and log level: " << Logger::logLevelToString(this->logLevel) << "." << endl;
}

Logger::Logger(const Logger& other) 
    : scope(other.scope), logLevel(other.logLevel) {
    cout << "Initialising logger with scope: " << this->scope << " and log level: " << Logger::logLevelToString(this->logLevel) << " using copy constructor." << endl;
}

Logger::~Logger()
{
    cout << "Destroying logger with scope: " << this->scope << "." << endl;
}

Logger& Logger::operator=(const Logger& other)
{
    cout << "Assigning logger with scope " << other.scope << " to logger with scope " << this->scope << "." << endl;
    this->scope = other.scope;
    this->logLevel = other.logLevel;
    return *this;
}

string Logger::logLevelToString(Logger::LogLevels logLevel)
{
    switch (logLevel) {
        case Logger::LogLevels::SILLY: return "SILLY";
        case Logger::LogLevels::VERBOSE: return "VERBOSE";
        case Logger::LogLevels::DEBUG: return "DEBUG";
        case Logger::LogLevels::INFO: return "INFO";
        case Logger::LogLevels::WARN: return "WARN";
        case Logger::LogLevels::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

void Logger::log(const Logger::LogLevels logLevel, const string& message) const
{
    if (logLevel < this->logLevel)
    {
        return;
    }
    cout << "[" << getCurrentDateTime() << "] [" << Logger::logLevelToString(logLevel) << "] (" << this->scope << ") : " << message << endl;
}

Logger::LogLevels Logger::getLogLevel()
{
    cout << "Getting log level." << endl;
    return this->logLevel;
}

void Logger::setLogLevel(Logger::LogLevels logLevel)
{
    cout << "Setting log level to: " << Logger::logLevelToString(logLevel) << "." << endl;
    this->logLevel = logLevel;
}

void Logger::silly(const string& message) const
{
    Logger::log(Logger::LogLevels::SILLY, message);
}

void Logger::verbose(const string& message) const
{
    Logger::log(Logger::LogLevels::VERBOSE, message);
}

void Logger::debug(const string& message) const
{
    Logger::log(Logger::LogLevels::DEBUG, message);
}

void Logger::info(const string& message) const
{
    Logger::log(Logger::LogLevels::INFO, message);
}

void Logger::warn(const string& message) const
{
    Logger::log(Logger::LogLevels::WARN, message);
}

void Logger::error(const string& message) const
{
    Logger::log(Logger::LogLevels::ERROR, message);
}
