#include <cstring>
#include <memory>
#include <iostream>
#include <fstream>
#include <mutex>
#include "../inc/BaseLogger.hpp"
#include "../inc/utils.hpp"
#include "../inc/StdoutLogger.hpp"

using namespace std;

StdoutLogger::StdoutLogger(const string &scope, const shared_ptr<BaseLogger::LifecycleLogResources> &lifecycle_log_resources)
    : BaseLogger::BaseLogger(scope, lifecycle_log_resources)
{
    this->logLifecycleMessage("[StdoutLogger] [" + string(this->scope) + "] [" + BaseLogger::logLevelToString(this->log_level) + " (default)]: Initialised.");
}

StdoutLogger::StdoutLogger(const string &scope, const BaseLogger::LogLevel log_level, const shared_ptr<BaseLogger::LifecycleLogResources> &lifecycle_log_resources)
    : BaseLogger::BaseLogger(scope, log_level, lifecycle_log_resources)
{
    this->logLifecycleMessage("[StdoutLogger] [" + string(this->scope) + "] [" + BaseLogger::logLevelToString(this->log_level) + "]: Initialised.");
}

StdoutLogger::StdoutLogger(const StdoutLogger &other) 
    : BaseLogger::BaseLogger(other), ansi_codes_enabled(other.ansi_codes_enabled), ansi_code_map(other.ansi_code_map)
{
    this->logLifecycleMessage("[StdoutLogger] [" + string(this->scope) + "] [" + BaseLogger::logLevelToString(this->log_level) + "]: Initialised with copy constructor.");
}

StdoutLogger::StdoutLogger(StdoutLogger &&other)
    : BaseLogger::BaseLogger(move(other)), ansi_codes_enabled(other.ansi_codes_enabled), ansi_code_map(move(other.ansi_code_map))
{
    other.ansi_codes_enabled = StdoutLogger::DEFAULT_ANSI_CODES_ENABLED;
    other.ansi_code_map = StdoutLogger::AnsiCodeMap();
    this->logLifecycleMessage("[StdoutLogger] [" + string(this->scope) + "] [" + BaseLogger::logLevelToString(this->log_level) + "]: Initialised with move constructor.");
}

StdoutLogger::~StdoutLogger()
{
    if (!this->scope)
    {
        this->logLifecycleMessage("[StdoutLogger]: Destroying moved-from instance.");
    }
    else
    {
        this->logLifecycleMessage("[StdoutLogger] [" + string(this->scope) + "]: Destroying instance.");
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
    this->logLifecycleMessage("[StdoutLogger] [" + string(other.scope) + "]: Assigning to [StdoutLogger] [" + string(this->scope) + "] with assignment operator.");
    if (this != &other)
    {
        BaseLogger::operator=(other);
        this->ansi_codes_enabled = other.ansi_codes_enabled;
        this->ansi_code_map = other.ansi_code_map;
    }
    else
    {
        this->logLifecycleMessage("[StdoutLogger] [" + string(other.scope) + "]: Self assignment detected. No-op.");
    }
    return *this;
}

StdoutLogger &StdoutLogger::operator=(StdoutLogger &&other)
{
    this->logLifecycleMessage("[StdoutLogger] [" + string(other.scope) + "]: Assigning to [StdoutLogger] [" + string(this->scope) + "] with move assignment operator.");
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
        this->logLifecycleMessage("[StdoutLogger] [" + string(other.scope) + "]: Self move assignment detected. No-op.");
    }
    return *this;
}


vector<AnsiCode> StdoutLogger::getTimestampAnsiCodes() const
{
    this->logLifecycleMessage("[StdoutLogger] [" + string(this->scope) + "]: Getting timestamp ANSI codes.");
    return this->ansi_code_map.timestamp;
}

void StdoutLogger::setTimestampAnsiCodes(const vector<AnsiCode> ansi_codes)
{
    this->logLifecycleMessage("[StdoutLogger] [" + string(this->scope) + "]: Setting timestamp ANSI codes.");
    this->ansi_code_map.timestamp = ansi_codes;
}

vector<AnsiCode> StdoutLogger::getScopeAnsiCodes() const
{
    this->logLifecycleMessage("[StdoutLogger] [" + string(this->scope) + "]: Getting scope ANSI codes.");
    return this->ansi_code_map.scope;
}

void StdoutLogger::setScopeAnsiCodes(const vector<AnsiCode> ansi_codes)
{
    this->logLifecycleMessage("[StdoutLogger] [" + string(this->scope) + "]: Setting scope ANSI codes.");
    this->ansi_code_map.scope = ansi_codes;
}

vector<AnsiCode> StdoutLogger::getLogLevelAnsiCodes(const BaseLogger::LogLevel log_level) const
{
    this->logLifecycleMessage("[StdoutLogger] [" + string(this->scope) + "]: Getting log level " + BaseLogger::logLevelToString(log_level) + " ANSI codes.");
    return this->ansi_code_map.level.at(log_level);
}

void StdoutLogger::setLogLevelAnsiCodes(const BaseLogger::LogLevel log_level, const vector<AnsiCode> ansi_codes)
{
    this->logLifecycleMessage("[StdoutLogger] [" + string(this->scope) + "]: Setting log level " + BaseLogger::logLevelToString(log_level) + " ANSI codes.");
    this->ansi_code_map.level[log_level] = ansi_codes;
}
