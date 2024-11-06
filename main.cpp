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
            }
            else
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
    Logger main_logger("main", Logger::LogLevel::SILLY);
    Logger summation_logger("summation", Logger::LogLevel::SILLY);

    // Set logger ANSI codes
    main_logger.info("Setting custom background color to main logger.");
    main_logger.setScopeAnsiCodes({ AnsiCode::YELLOW });
    main_logger.info("Setting custom timestamp and scope colors on summation logger.");
    summation_logger.setTimestampAnsiCodes({ AnsiCode::CYAN });
    summation_logger.setScopeAnsiCodes({ AnsiCode::UNDERLINE, AnsiCode::MAGENTA });

    // Test logger output
    summation_logger.verbose("Running summation...");
    int sum = runSummation(&summation_logger);
    summation_logger.verbose("Summation result is " + to_string(sum) + ".");

    // Disable logger ANSI codes
    main_logger.info("Disabling ANSI codes on summation logger.");
    summation_logger.ansi_codes_enabled = false;
    summation_logger.info("I lost my colors...");

    // Test assignment operator
    main_logger.info("Assigning summation logger to main logger.");
    main_logger.debug("This is a main logger message before being assigned to the summation logger.");
    main_logger = summation_logger;
    main_logger.debug("This is a main logger message after being assigned to the summation logger.");

    return 0;
}
