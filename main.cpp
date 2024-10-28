#include "Logger.hpp"

using namespace std;

int runSummation(const Logger* logger) {
    int sum = 0, number;
    string input;
    logger->info("Enter numbers to sum, end to exit.");
    while (true) {
        cout << "> ";
        getline(cin, input);
        try {
            number = stoi(input);
            if (number == 0) {
                logger->warn("Input is 0. Sum unchanged.");
            }
            if (number < 0) {
                logger->warn("Negative input. Sum decreasing.");
            } else {
                logger->silly("Input is " + to_string(number) + ".");
            }
            sum += number;
            logger->debug("Current sum: " + to_string(sum) + ".");
        } catch (invalid_argument&) {
            if (input == "end") {
                logger->info("Ending summation.");
                break;
            } else {
                logger->error("Invalid input \"" + input + "\"!");
            }
        }
    }
    logger->debug("Returning " + to_string(sum) + ".");
    return sum;
}

int main() {
    Logger summationLogger("main-summation");

    int sum = runSummation(&summationLogger);
    summationLogger.verbose("Summation result is " + to_string(sum) + ".");

    return 0;
}
