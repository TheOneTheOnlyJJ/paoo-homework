#include "../inc/Logger.hpp"
#include "../inc/utils.hpp"

using namespace std;

// Lifecycle management
Logger::Logger(const string& scope)
    : scope(scope), log_level(Logger::DEFAULT_LOG_LEVEL)
{
    cout << "Initialising logger with scope: " << this->scope << " and default log level: " << Logger::logLevelToString(this->log_level) << "." << endl;
}

Logger::Logger(const string& scope, const Logger::LogLevel log_level)
    : scope(scope), log_level(log_level)
{
    cout << "Initialising logger with scope: " << this->scope << " and log level: " << Logger::logLevelToString(this->log_level) << "." << endl;
}

Logger::Logger(const Logger& other) 
    : scope(other.scope), log_level(other.log_level)
{
    cout << "Initialising logger with scope: " << this->scope << " and log level: " << Logger::logLevelToString(this->log_level) << " using copy constructor." << endl;
}

Logger::~Logger()
{
    cout << "Destroying logger with scope: " << this->scope << "." << endl;
}

// Operator overloads
Logger& Logger::operator=(const Logger& other)
{
    cout << "Assigning logger with scope " << other.scope << " to logger with scope " << this->scope << "." << endl;
    this->scope = other.scope;
    this->log_level = other.log_level;
    this->ansi_code_map = other.ansi_code_map;
    return *this;
}

// Log level methods
Logger::LogLevel Logger::getLogLevel() const
{
    cout << "Getting log level." << endl;
    return this->log_level;
}

void Logger::setLogLevel(Logger::LogLevel log_level)
{
    cout << "Setting log level to: " << Logger::logLevelToString(log_level) << "." << endl;
    this->log_level = log_level;
}


string Logger::logLevelToString(Logger::LogLevel log_level)
{
    switch (log_level) {
        case Logger::LogLevel::SILLY:
            return "SILLY";
        case Logger::LogLevel::VERBOSE:
            return "VERBOSE";
        case Logger::LogLevel::DEBUG:
            return "DEBUG";
        case Logger::LogLevel::INFO:
            return "INFO";
        case Logger::LogLevel::WARN:
            return "WARN";
        case Logger::LogLevel::ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

void Logger::log(const Logger::LogLevel log_level, const string& message) const
{
    if (log_level < this->log_level)
    {
        return;
    }
    if (this->use_ansi_codes)
    {
        cout << wrapWithAnsiCodes(this->ansi_code_map.timestamp, "[" + getCurrentDateTime() + "]") << " " << wrapWithAnsiCodes(this->ansi_code_map.level.at(log_level), "(" + Logger::logLevelToString(log_level) + ")") << " " << wrapWithAnsiCodes(this->ansi_code_map.scope, "(" + this->scope + ")") << ": " << message << endl;
        return;
    }
    cout << "[" << getCurrentDateTime() << "] [" << Logger::logLevelToString(log_level) << "] (" << this->scope << ") : " << message << endl;
}

// Log methods
void Logger::silly(const string& message) const
{
    Logger::log(Logger::LogLevel::SILLY, message);
}

void Logger::verbose(const string& message) const
{
    Logger::log(Logger::LogLevel::VERBOSE, message);
}

void Logger::debug(const string& message) const
{
    Logger::log(Logger::LogLevel::DEBUG, message);
}

void Logger::info(const string& message) const
{
    Logger::log(Logger::LogLevel::INFO, message);
}

void Logger::warn(const string& message) const
{
    Logger::log(Logger::LogLevel::WARN, message);
}

void Logger::error(const string& message) const
{
    Logger::log(Logger::LogLevel::ERROR, message);
}
