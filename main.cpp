#include "include/Logger.h"

using namespace std;

void runAddition(const Logger* logger) {
    int accumulator = 0, number;
    string input;
    logger->info("Enter numbers to sum, 0 to end.");
    while (true) {
        cout << "> ";
        getline(cin, input);
        try {
            number = stoi(input);
            if (number == 0) {
                logger->info("Input is 0. Exiting input loop.");
                break;
            }
            if (number < 0) {
                logger->warn("Negative input. Sum decreasing.");
            }
            accumulator += number;
            logger->debug("Current sum: " + to_string(accumulator) + ".");
        } catch (invalid_argument&) {
            logger->error("Invalid input \"" + input + "\"!");
        }
    }
    logger->info("Final sum: " + to_string(accumulator) + ".");
}

int main() {
    Logger additionLogger("main-addition");

    runAddition(&additionLogger);

    return 0;
}
