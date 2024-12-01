#include <cstring>
#include <memory>
#include "../inc/FileLogger.hpp"

using namespace std;

FileLogger::FileLogger(const string &scope, unique_ptr<ofstream> log_file_stream, const shared_ptr<BaseLogger::LifecycleLogResources> &lifecycle_log_resources)
    : BaseLogger::BaseLogger(scope, lifecycle_log_resources), log_file_stream(move(log_file_stream))
{
    this->logLifecycleMessage("[FileLogger] [" + string(this->scope) + "] [" + BaseLogger::logLevelToString(this->log_level) + " (default)]: Initialised.");
}

FileLogger::FileLogger(const string &scope, unique_ptr<ofstream> log_file_stream, const BaseLogger::LogLevel log_level, const shared_ptr<BaseLogger::LifecycleLogResources> &lifecycle_log_resources)
    : BaseLogger::BaseLogger(scope, log_level, lifecycle_log_resources), log_file_stream(move(log_file_stream))
{
    this->logLifecycleMessage("[FileLogger] [" + string(this->scope) + "] [" + BaseLogger::logLevelToString(this->log_level) + "]: Initialised.");
}

FileLogger::FileLogger(FileLogger &&other)
    : BaseLogger::BaseLogger(move(other)), log_file_stream(move(other.log_file_stream))
{
    this->logLifecycleMessage("[FileLogger] [" + string(this->scope) + "] [" + BaseLogger::logLevelToString(this->log_level) + "]: Initialised with move constructor.");
}

FileLogger::~FileLogger()
{
    if (!this->scope)
    {
        this->logLifecycleMessage("[FileLogger]: Destroying moved-from instance.");
    }
    else
    {
        this->logLifecycleMessage("[FileLogger] [" + string(this->scope) + "]: Destroying instance.");
    }
    if (this->log_file_stream && this->log_file_stream->is_open())
    {
        this->log_file_stream->close();
    }
}

FileLogger &FileLogger::operator=(FileLogger &&other)
{
    this->logLifecycleMessage("[FileLogger] [" + string(other.scope) + "]: Assigning to [FileLogger] [" + string(this->scope) + "] with move assignment operator.");
    if (this != &other)
    {
        BaseLogger::operator=(move(other));
        this->log_file_stream = move(other.log_file_stream);
        other.log_file_stream = nullptr;
    }
    else
    {
        this->logLifecycleMessage("[FileLogger] [" + string(other.scope) + "]: Self move assignment detected. No-op.");
    }
    return *this;
}

void FileLogger::log(const BaseLogger::LogLevel log_level, const string &message) const
{
    if (log_level < this->log_level || !this->log_file_stream || !this->log_file_stream->is_open())
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
    
    *this->log_file_stream << TIMESTAMP << " " << LOG_LEVEL << " " << SCOPE << ": " << message << endl;
}
