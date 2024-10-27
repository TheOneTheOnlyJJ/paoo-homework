#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>

using namespace std;

class Logger {
private:
    string scope;

    void log(const string& level, const string& message) const;

public:
    explicit Logger(const string& scope);
    
    ~Logger();

    void silly(const string& message) const;
    void verbose(const string& message) const;
    void debug(const string& message) const;
    void info(const string& message) const;
    void warn(const string& message) const;
    void error(const string& message) const;
};

#endif // LOGGER_H
