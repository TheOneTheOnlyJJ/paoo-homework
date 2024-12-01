#include <memory>
#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>
#include "BaseLogger.hpp"
#include "StdoutLogger.hpp"
#include "FileLogger.hpp"
#include "utils.hpp"

using namespace std;

int runSummation(const BaseLogger* logger)
{
    int sum = 0;
    string input;
    cout << "Enter integers to sum, end to exit." << endl;
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

void testLoggers(const shared_ptr<BaseLogger::LifecycleLogResources> &lifecycle_log_resources)
{
    cout << "Beginning logger tests!" << endl;

    // Test constructors
    StdoutLogger test_logger_1("test-1", BaseLogger::LogLevel::SILLY, lifecycle_log_resources);
    test_logger_1.info("I am instance test-1, initialised with the SILLY log level instead of the default.");

    // Test move constructor
    StdoutLogger test_logger_2 = move(test_logger_1);
    test_logger_2.info("I am instance test-2, but initialised by moving test-1 into me.");

    // Test move assignment operator
    StdoutLogger test_logger_3("test-3", lifecycle_log_resources);
    test_logger_3.info("I am instance test-3, but test-2 will move into me.");
    test_logger_3 = move(test_logger_2);
    test_logger_3.info("I am instance test-3, but test-2 moved into me.");

    // Test assignment operator
    StdoutLogger test_logger_4("test-4", lifecycle_log_resources);
    test_logger_4.info("I am instance test-4 and test-3 will get assigned to me.");
    test_logger_4 = test_logger_3;
    test_logger_4.info("I am instance test-4, but test-3 was assigned to me.");

    // Test polymorphism
    BaseLogger *maybe_base_logger = new StdoutLogger("maybe-base", lifecycle_log_resources);
    maybe_base_logger->info("I am instance maybe-base, typed as BaseLogger* but initialised as StdoutLogger. Let's see where this message ends up.");
    delete maybe_base_logger;

    cout << "Logger tests done!." << endl;
}

int main()
{
    const shared_ptr<BaseLogger::LifecycleLogResources> lifecycle_log_resources = 
        make_shared<BaseLogger::LifecycleLogResources>(make_shared<ofstream>("./logs/loggerLifecycleLogs.txt"), make_shared<mutex>());

    // Start a thread to test loggers
    thread testLoggersThread(testLoggers, lifecycle_log_resources);

    // Run summation
    unique_ptr<ofstream> log_file_stream = make_unique<ofstream>("./logs/summationLogs.txt");
    FileLogger summation_logger("summation", move(log_file_stream), BaseLogger::LogLevel::SILLY, lifecycle_log_resources);
    summation_logger.verbose("Running summation:");
    int sum = runSummation(&summation_logger);
    summation_logger.verbose("Summation done! Result: " + to_string(sum) + ".");

    // Wait fot the test thread to finish
    testLoggersThread.join();

    return 0;
}
