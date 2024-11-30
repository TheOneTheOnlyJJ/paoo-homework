#include "../inc/utils.hpp"

using namespace std;

string getAnsiCode(const AnsiCode color)
{
    return "\033[" + to_string((uint8_t)color) + "m";
}

string wrapWithAnsiCodes(const vector<AnsiCode> &ansi_codes, const string &target)
{
    ostringstream oss;
    for (const AnsiCode &code : ansi_codes)
    {
        oss << getAnsiCode(code);
    }
    oss << target;
    oss << getAnsiCode(AnsiCode::RESET);
    return oss.str();
}

string getCurrentDateTime()
{
    time_t now = time(nullptr);
    tm* now_tm = localtime(&now);

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", now_tm);

    return string(buffer);
}