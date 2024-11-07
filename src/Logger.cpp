#include <cstring>
#include "../inc/Logger.hpp"
#include "../inc/utils.hpp"

using namespace std;

Logger::Logger(const string& scope)
    : scope((char *) malloc((scope.length() + 1) * sizeof(char))), log_level(Logger::DEFAULT_LOG_LEVEL)
{
    if (!this->scope) {
        exit(EXIT_FAILURE);
    }
    strcpy(this->scope, scope.c_str());
    cout << "Initialised logger with scope: " << this->scope << " and default log level: " << Logger::logLevelToString(this->log_level) << "." << endl;
}

Logger::Logger(const string& scope, const Logger::LogLevel log_level)
    : scope((char *) malloc((scope.length() + 1) * sizeof(char))), log_level(log_level)
{
    if (!this->scope) {
        exit(EXIT_FAILURE);
    }
    strcpy(this->scope, scope.c_str());
    cout << "Initialised logger with scope: " << this->scope << " and log level: " << Logger::logLevelToString(this->log_level) << "." << endl;
}

Logger::Logger(const Logger& other) 
    : scope((char *) malloc((strlen(other.scope) + 1) * sizeof(char))), log_level(other.log_level)
{
    if (!this->scope) {
        exit(EXIT_FAILURE);
    }
    strcpy(this->scope, other.scope);
    cout << "Initialised logger with scope: " << this->scope << " and log level: " << Logger::logLevelToString(this->log_level) << " using copy constructor." << endl;
}

Logger::~Logger()
{
    cout << "Destroying logger with scope: " << this->scope << "." << endl;
    free(this->scope);
}

Logger& Logger::operator=(const Logger& other)
{
    cout << "Assigning logger with scope " << other.scope << " to logger with scope " << this->scope << "." << endl;
    this->scope = (char *) malloc((strlen(other.scope) + 1) * sizeof(char));
    if (!this->scope) {
        exit(EXIT_FAILURE);
    }
    strcpy(this->scope, other.scope);
    this->log_level = other.log_level;
    this->ansi_code_map = other.ansi_code_map;
    this->ansi_codes_enabled = other.ansi_codes_enabled;
    return *this;
}

string Logger::logLevelToString(const Logger::LogLevel log_level)
{
    switch (log_level)
    {
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
    // Format log message components
    string TIMESTAMP = "[" + getCurrentDateTime() + "]";
    string LOG_LEVEL = "[" + Logger::logLevelToString(log_level) + "]";
    // Scope requires c-string handling
    const size_t SCOPE_BUFFER_LENGTH = strlen(this->scope) + 3;
    char *SCOPE_BUFFER = (char *) malloc(SCOPE_BUFFER_LENGTH * sizeof(char));
    if (!SCOPE_BUFFER) {
        exit(EXIT_FAILURE);
    }
    SCOPE_BUFFER[0] = '(';
    SCOPE_BUFFER[1] = '\0';
    strcat(SCOPE_BUFFER, this->scope);
    SCOPE_BUFFER[SCOPE_BUFFER_LENGTH - 2] = ')';
    SCOPE_BUFFER[SCOPE_BUFFER_LENGTH - 1] = '\0';
    string SCOPE = SCOPE_BUFFER;
    
    if (this->ansi_codes_enabled)
    {
        TIMESTAMP = wrapWithAnsiCodes(this->ansi_code_map.timestamp, TIMESTAMP);
        LOG_LEVEL = wrapWithAnsiCodes(this->ansi_code_map.level.at(log_level), LOG_LEVEL);
        SCOPE = wrapWithAnsiCodes(this->ansi_code_map.scope, SCOPE);
    }
    cout << TIMESTAMP << " " << LOG_LEVEL << " " << SCOPE << ": " << message << endl;
}

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

Logger::LogLevel Logger::getLogLevel() const
{
    cout << "Getting log level." << endl;
    return this->log_level;
}

void Logger::setLogLevel(const Logger::LogLevel log_level)
{
    cout << "Setting log level to: " << Logger::logLevelToString(log_level) << "." << endl;
    this->log_level = log_level;
}

vector<AnsiCode> Logger::getTimestampAnsiCodes() const
{
    cout << "Getting timestamp ANSI codes." << endl;
    return this->ansi_code_map.timestamp;
}

void Logger::setTimestampAnsiCodes(const vector<AnsiCode> ansi_codes)
{
    cout << "Setting timestamp ANSI codes." << endl;
    this->ansi_code_map.timestamp = ansi_codes;
}

vector<AnsiCode> Logger::getScopeAnsiCodes() const
{
    cout << "Getting scope ANSI codes." << endl;
    return this->ansi_code_map.scope;
}

void Logger::setScopeAnsiCodes(const vector<AnsiCode> ansi_codes)
{
    cout << "Setting scope ANSI codes." << endl;
    this->ansi_code_map.scope = ansi_codes;
}

vector<AnsiCode> Logger::getLogLevelAnsiCodes(const Logger::LogLevel log_level) const
{
    cout << "Getting log level " << Logger::logLevelToString(log_level) << " ANSI codes." << endl;
    return this->ansi_code_map.level.at(log_level);
}

void Logger::setLogLevelAnsiCodes(const Logger::LogLevel log_level, const vector<AnsiCode> ansi_codes)
{
    cout << "Setting log level " << Logger::logLevelToString(log_level) << " ANSI codes." << endl;
    this->ansi_code_map.level[log_level] = ansi_codes;
}
