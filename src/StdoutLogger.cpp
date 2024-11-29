#include <cstring>
#include <memory>
#include <iostream>
#include <fstream>
#include <mutex>
#include "../inc/BaseLogger.hpp"
#include "../inc/utils.hpp"
#include "../inc/StdoutLogger.hpp"

using namespace std;

StdoutLogger::StdoutLogger(const string& scope, const shared_ptr<ofstream>& lifecycle_log_file_stream, const shared_ptr<mutex>& lifecycle_log_mutex)
    : BaseLogger::BaseLogger(scope, lifecycle_log_file_stream, lifecycle_log_mutex)
{
    this->logLifecycleMessage("Initialised StdoutLogger with scope: " + string(this->scope) + " and default log level: " + BaseLogger::logLevelToString(this->log_level) + ".");
}

StdoutLogger::StdoutLogger(const string& scope, const BaseLogger::LogLevel log_level, const shared_ptr<ofstream>& lifecycle_log_file_stream, const shared_ptr<mutex>& lifecycle_log_mutex)
    : BaseLogger::BaseLogger(scope, log_level, lifecycle_log_file_stream, lifecycle_log_mutex)
{
    this->logLifecycleMessage("Initialised StdoutLogger with scope: " + string(this->scope) + " and log level: " + BaseLogger::logLevelToString(this->log_level) + ".");
}

StdoutLogger::StdoutLogger(const StdoutLogger& other) 
    : BaseLogger::BaseLogger(other), ansi_codes_enabled(other.ansi_codes_enabled), ansi_code_map(other.ansi_code_map)
{
    this->logLifecycleMessage("Initialised StdoutLogger with scope: " + string(this->scope) + " and log level: " + BaseLogger::logLevelToString(this->log_level) + " using copy constructor.");
}

StdoutLogger::StdoutLogger(StdoutLogger&& other)
    : BaseLogger::BaseLogger(move(other)), ansi_codes_enabled(other.ansi_codes_enabled), ansi_code_map(move(other.ansi_code_map))
{
    other.ansi_codes_enabled = StdoutLogger::DEFAULT_ANSI_CODES_ENABLED;
    other.ansi_code_map = StdoutLogger::AnsiCodeMap();
    this->logLifecycleMessage("Initialised StdoutLogger with scope: " + string(this->scope) + " and log level: " + BaseLogger::logLevelToString(this->log_level) + " using move constructor.");
}

StdoutLogger::~StdoutLogger()
{
    if (!this->scope)
    {
        this->logLifecycleMessage("Destroying moved-from StdoutLogger.");
    }
    else
    {
        this->logLifecycleMessage("Destroying StdoutLogger with scope: " + string(this->scope) + ".");
    }
}

void StdoutLogger::log(const BaseLogger::LogLevel log_level, const string &message) const
{
    if (log_level < this->log_level)
    {
        return;
    }
    // Format log message components
    string TIMESTAMP = "[" + getCurrentDateTime() + "]";
    string LOG_LEVEL = "[" + BaseLogger::logLevelToString(log_level) + "]";
    // Scope requires c-string handling
    const size_t SCOPE_BUFFER_LENGTH = strlen(this->scope) + 3;
    char *SCOPE_BUFFER = (char *) malloc(SCOPE_BUFFER_LENGTH * sizeof(char));
    if (!SCOPE_BUFFER)
    {
        exit(EXIT_FAILURE);
    }
    SCOPE_BUFFER[0] = '(';
    SCOPE_BUFFER[1] = '\0';
    strcat(SCOPE_BUFFER, this->scope);
    SCOPE_BUFFER[SCOPE_BUFFER_LENGTH - 2] = ')';
    SCOPE_BUFFER[SCOPE_BUFFER_LENGTH - 1] = '\0';
    string SCOPE = SCOPE_BUFFER;
    free(SCOPE_BUFFER);
    
    if (this->ansi_codes_enabled)
    {
        TIMESTAMP = wrapWithAnsiCodes(this->ansi_code_map.timestamp, TIMESTAMP);
        LOG_LEVEL = wrapWithAnsiCodes(this->ansi_code_map.level.at(log_level), LOG_LEVEL);
        SCOPE = wrapWithAnsiCodes(this->ansi_code_map.scope, SCOPE);
    }
    cout << TIMESTAMP << " " << LOG_LEVEL << " " << SCOPE << ": " << message << endl;
}

StdoutLogger &StdoutLogger::operator=(const StdoutLogger &other)
{
    this->logLifecycleMessage("Assigning StdoutLogger with scope " + string(other.scope) + " to StdoutLogger with scope " + string(this->scope) + ".");
    if (this != &other)
    {
        BaseLogger::operator=(other);
        this->ansi_codes_enabled = other.ansi_codes_enabled;
        this->ansi_code_map = other.ansi_code_map;
    }
    else
    {
        this->logLifecycleMessage("Self-assignment detected. No-op.");
    }
    return *this;
}

StdoutLogger& StdoutLogger::operator=(StdoutLogger&& other)
{
    this->logLifecycleMessage("Assigning StdoutLogger with scope " + string(other.scope) + " to StdoutLogger with scope " + string(this->scope) + " using move assignment operator.");
    if (this != &other)
    {
        BaseLogger::operator=(move(other));
        this->ansi_codes_enabled = other.ansi_codes_enabled;
        this->ansi_code_map = move(other.ansi_code_map);
        other.ansi_codes_enabled = StdoutLogger::DEFAULT_ANSI_CODES_ENABLED;
        other.ansi_code_map = StdoutLogger::AnsiCodeMap();
    }
    else
    {
        this->logLifecycleMessage("Self-assignment detected. No-op.");
    }
    return *this;
}


vector<AnsiCode> StdoutLogger::getTimestampAnsiCodes() const
{
    this->logLifecycleMessage("Getting timestamp ANSI codes.");
    return this->ansi_code_map.timestamp;
}

void StdoutLogger::setTimestampAnsiCodes(const vector<AnsiCode> ansi_codes)
{
    this->logLifecycleMessage("Setting timestamp ANSI codes.");
    this->ansi_code_map.timestamp = ansi_codes;
}

vector<AnsiCode> StdoutLogger::getScopeAnsiCodes() const
{
    this->logLifecycleMessage("Getting scope ANSI codes.");
    return this->ansi_code_map.scope;
}

void StdoutLogger::setScopeAnsiCodes(const vector<AnsiCode> ansi_codes)
{
    this->logLifecycleMessage("Setting scope ANSI codes.");
    this->ansi_code_map.scope = ansi_codes;
}

vector<AnsiCode> StdoutLogger::getLogLevelAnsiCodes(const BaseLogger::LogLevel log_level) const
{
    this->logLifecycleMessage("Getting log level " + BaseLogger::logLevelToString(log_level) + " ANSI codes.");
    return this->ansi_code_map.level.at(log_level);
}

void StdoutLogger::setLogLevelAnsiCodes(const BaseLogger::LogLevel log_level, const vector<AnsiCode> ansi_codes)
{
    this->logLifecycleMessage("Setting log level " + BaseLogger::logLevelToString(log_level) + " ANSI codes.");
    this->ansi_code_map.level[log_level] = ansi_codes;
}
