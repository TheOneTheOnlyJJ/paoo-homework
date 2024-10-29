#include "Logger.hpp"
#include "utils.hpp"

using namespace std;

int runSummation(const Logger* logger)
{
    int sum = 0;
    string input;
    logger->info("Enter integers to sum, end to exit.");
    while (true)
    {
        cout << "> ";
        getline(cin, input);
        logger->silly("Input is \"" + input + "\".");
        try
        {
            int number = stoi(input);
            if (number == 0)
            {
                logger->warn("Input is \"0\". Sum unchanged.");
            }
            else if (number < 0)
            {
                logger->warn("Negative input. Sum decreasing.");
            }
            sum += number;
            logger->debug("Current sum: " + to_string(sum) + ".");
        }
        catch (invalid_argument&)
        {
            if (input == "end")
            {
                logger->info("Ending summation.");
                break;
            } else
            {
                logger->error("Invalid input \"" + input + "\"!");
            }
        }
    }
    logger->debug("Returning " + to_string(sum) + ".");
    return sum;
}

int main()
{
    // Initialise loggers
    Logger mainLogger("main", Logger::LogLevel::SILLY);
    Logger summationLogger("summation", Logger::LogLevel::SILLY);

    // Test loggers
    mainLogger.verbose("Running summation...");
    int sum = runSummation(&summationLogger);
    mainLogger.verbose("Summation result is " + to_string(sum) + ".");

    // Test assignment operator
    mainLogger.debug("This is a main logger message before being assigned to the summation logger.");
    mainLogger = summationLogger;
    mainLogger.debug("This is a main logger message after being assigned to the summation logger.");

    // TODO: Break up very long line in log method. Make ansi code map private, add getters and setters for the specific components. Add constructors that include the ansi code map.
    return 0;
}
