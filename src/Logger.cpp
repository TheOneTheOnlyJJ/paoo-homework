#include "../include/Logger.h"
#include "../include/utils.h"

using namespace std;

Logger::Logger(const string& scope)
    : scope(scope) {
    cout << "Initialising logger with scope: " << scope << endl;
}

Logger::~Logger() {
    cout << "Destroying logger with scope: " << scope << endl;
}

void Logger::log(const string& level, const string& message) const {
    cout << "[" << getCurrentDateTime() << "] [" << level << "] (" << scope << ") : " << message << endl;
}

void Logger::silly(const string& message) const {
    Logger::log("silly", message);
}

void Logger::verbose(const string& message) const {
    Logger::log("verbose", message);
}

void Logger::debug(const string& message) const {
    Logger::log("debug", message);
}

void Logger::info(const string& message) const {
    Logger::log("info", message);
}

void Logger::warn(const string& message) const {
    Logger::log("warn", message);
}

void Logger::error(const string& message) const {
    Logger::log("error", message);
}
