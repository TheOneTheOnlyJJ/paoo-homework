#include <memory>
#include <iostream>
#include <fstream>
#include <mutex>
#include "BaseLogger.hpp"
#include "StdoutLogger.hpp"
#include "utils.hpp"

using namespace std;

int runSummation(const BaseLogger* logger)
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
    shared_ptr<ofstream> lifecycle_log_file_stream = make_shared<ofstream>("./logs/loggerLifecycleLogs.txt");
    shared_ptr<mutex> lifecycle_log_mutex = make_shared<mutex>();
    // Initialise loggers
    StdoutLogger main_logger("main", BaseLogger::LogLevel::SILLY, lifecycle_log_file_stream, lifecycle_log_mutex);
    StdoutLogger summation_logger("summation", BaseLogger::LogLevel::SILLY, lifecycle_log_file_stream, lifecycle_log_mutex);
    BaseLogger *maybe_base_logger = new StdoutLogger("maybe-base", lifecycle_log_file_stream, lifecycle_log_mutex);

    // Set logger ANSI codes
    // main_logger.info("Setting custom color to main logger.");
    // main_logger.setScopeAnsiCodes({ AnsiCode::YELLOW });
    // main_logger.info("Setting custom timestamp and scope colors on summation logger.");
    // summation_logger.setTimestampAnsiCodes({ AnsiCode::CYAN });
    // summation_logger.setScopeAnsiCodes({ AnsiCode::UNDERLINE, AnsiCode::MAGENTA });

    // Test logger output
    // summation_logger.verbose("Running summation...");
    // int sum = runSummation(&summation_logger);
    // summation_logger.verbose("Summation result is " + to_string(sum) + ".");

    // Disable logger ANSI codes
    // main_logger.info("Disabling ANSI codes on summation logger.");
    // summation_logger.ansi_codes_enabled = false;
    // summation_logger.info("I lost my colors...");

    // Test assignment operator
    // main_logger.info("Assigning summation logger to main logger.");
    // main_logger.debug("This is a main logger message before being assigned to the summation logger.");
    // main_logger = summation_logger;
    // main_logger.debug("This is a main logger message after being assigned to the summation logger.");

    // Test move constructor
    StdoutLogger main_logger_2 = move(main_logger);
    main_logger_2.warn("I am main logger 2, just moved from main logger.");
    StdoutLogger main_logger_3("main-3", lifecycle_log_file_stream, lifecycle_log_mutex);
    main_logger_3.info("Main logger 2 will move into me.");
    main_logger_3 = move(main_logger_2);

    // Test polymorphism
    maybe_base_logger->info("I'm the maybe base logger.");
    delete maybe_base_logger;

    // TODO: Add a thread to test the mutex
    // TODO: Add unique_ptr somewhere
    return 0;
}
