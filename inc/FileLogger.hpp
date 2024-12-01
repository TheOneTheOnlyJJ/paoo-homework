#pragma once

#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include <fstream>
#include <mutex>
#include "utils.hpp"
#include "BaseLogger.hpp"

using namespace std;

class FileLogger : public BaseLogger
{
public:
    // Static attributes
    // Static methods
    // Instance attributes
    // Constructors & destructors
    FileLogger() = delete;
    explicit FileLogger(const string &scope, unique_ptr<ofstream> log_file_stream, const shared_ptr<BaseLogger::LifecycleLogResources> &lifecycle_log_resources);
    FileLogger(const string &scope, unique_ptr<ofstream> log_file_stream, const BaseLogger::LogLevel log_level, const shared_ptr<BaseLogger::LifecycleLogResources> &lifecycle_log_resources);
    FileLogger(const FileLogger &other) = delete;
    FileLogger(FileLogger &&other);
    ~FileLogger();
    // Instance methods
    FileLogger &operator=(const FileLogger &other) = delete;
    FileLogger &operator=(FileLogger &&other);
protected:
    // Static attributes
    // Static methods
    // Instance attributes
    unique_ptr<ofstream> log_file_stream;
    // Instance methods
    void log(const BaseLogger::LogLevel log_level, const string &message) const override;
private:
    // Static attributes
    // Static methods
    // Instance attributes
    // Instance methods
};
