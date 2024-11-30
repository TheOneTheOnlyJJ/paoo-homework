#pragma once

#include <string>
#include <vector>
#include <ctime>
#include <iomanip>

using namespace std;

enum class AnsiCode : uint8_t
{
    RESET       = 0,
    BOLD        = 1,
    UNDERLINE   = 4,

    // Foreground colors
    BLACK       = 30,
    RED         = 31,
    GREEN       = 32,
    YELLOW      = 33,
    BLUE        = 34,
    MAGENTA     = 35,
    CYAN        = 36,
    WHITE       = 37,

    // Background colors
    BG_BLACK     = 40,
    BG_RED       = 41,
    BG_GREEN     = 42,
    BG_YELLOW    = 43,
    BG_BLUE      = 44,
    BG_MAGENTA   = 45,
    BG_CYAN      = 46,
    BG_WHITE     = 47
};


string getAnsiCode(const AnsiCode color);
string wrapWithAnsiCodes(const vector<AnsiCode> &ansi_codes, const string &target);
string getCurrentDateTime();
