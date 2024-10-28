#include "../inc/utils.hpp"

using namespace std;

string getCurrentDateTime() {
    time_t now = time(nullptr);
    tm* now_tm = localtime(&now);

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", now_tm);

    return string(buffer);
}