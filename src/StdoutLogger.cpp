#include <cstring>
#include "../inc/BaseLogger.hpp"
#include "../inc/utils.hpp"
#include "../inc/StdoutLogger.hpp"

using namespace std;

StdoutLogger::StdoutLogger(const string& scope)
    : BaseLogger::BaseLogger(scope)
{
    cout << "Initialised StdoutLogger with scope: " << this->scope << " and default log level: " << BaseLogger::logLevelToString(this->log_level) << "." << endl;
}

StdoutLogger::StdoutLogger(const string& scope, const BaseLogger::LogLevel log_level)
    : BaseLogger::BaseLogger(scope, log_level)
{
    cout << "Initialised StdoutLogger with scope: " << this->scope << " and log level: " << BaseLogger::logLevelToString(this->log_level) << "." << endl;
}

StdoutLogger::StdoutLogger(const StdoutLogger& other) 
    : BaseLogger::BaseLogger(other), ansi_codes_enabled(other.ansi_codes_enabled), ansi_code_map(other.ansi_code_map)
{
    cout << "Initialised StdoutLogger with scope: " << this->scope << " and log level: " << BaseLogger::logLevelToString(this->log_level) << " using copy constructor." << endl;
}

StdoutLogger::StdoutLogger(StdoutLogger&& other)
    : BaseLogger::BaseLogger(move(other)), ansi_codes_enabled(other.ansi_codes_enabled), ansi_code_map(move(other.ansi_code_map))
{
    other.ansi_codes_enabled = StdoutLogger::DEFAULT_ANSI_CODES_ENABLED;
    other.ansi_code_map = StdoutLogger::AnsiCodeMap();
    cout << "Initialised StdoutLogger with scope: " << this->scope << " and log level: " << BaseLogger::logLevelToString(this->log_level) << " using move constructor." << endl;
}

StdoutLogger::~StdoutLogger()
{
    if (!this->scope)
    {
        cout << "Destroying moved-from BaseLogger." << endl;
    }
    else
    {
        cout << "Destroying StdoutLogger with scope: " << this->scope << "." << endl;
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
    cout << "Assigning StdoutLogger with scope " << other.scope << " to StdoutLogger with scope " << this->scope << "." << endl;
    if (this != &other)
    {
        BaseLogger::operator=(other);
        this->ansi_codes_enabled = other.ansi_codes_enabled;
        this->ansi_code_map = other.ansi_code_map;
    }
    else
    {
        cout << "Self-assignment detected. No-op." << endl;
    }
    return *this;
}

StdoutLogger& StdoutLogger::operator=(StdoutLogger&& other)
{
    cout << "Assigning StdoutLogger with scope " << other.scope << " to StdoutLogger with scope " << this->scope << " using move assignment operator." << endl;
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
        cout << "Self-assignment detected. No-op." << endl;
    }
    return *this;
}


vector<AnsiCode> StdoutLogger::getTimestampAnsiCodes() const
{
    cout << "Getting timestamp ANSI codes." << endl;
    return this->ansi_code_map.timestamp;
}

void StdoutLogger::setTimestampAnsiCodes(const vector<AnsiCode> ansi_codes)
{
    cout << "Setting timestamp ANSI codes." << endl;
    this->ansi_code_map.timestamp = ansi_codes;
}

vector<AnsiCode> StdoutLogger::getScopeAnsiCodes() const
{
    cout << "Getting scope ANSI codes." << endl;
    return this->ansi_code_map.scope;
}

void StdoutLogger::setScopeAnsiCodes(const vector<AnsiCode> ansi_codes)
{
    cout << "Setting scope ANSI codes." << endl;
    this->ansi_code_map.scope = ansi_codes;
}

vector<AnsiCode> StdoutLogger::getLogLevelAnsiCodes(const BaseLogger::LogLevel log_level) const
{
    cout << "Getting log level " << BaseLogger::logLevelToString(log_level) << " ANSI codes." << endl;
    return this->ansi_code_map.level.at(log_level);
}

void StdoutLogger::setLogLevelAnsiCodes(const BaseLogger::LogLevel log_level, const vector<AnsiCode> ansi_codes)
{
    cout << "Setting log level " << BaseLogger::logLevelToString(log_level) << " ANSI codes." << endl;
    this->ansi_code_map.level[log_level] = ansi_codes;
}
