#include "debug.h"

Debug::Logger::Logger(std::ostream& stream)
{
    if (stream.good())
        outputFileStream = &stream;
    else
        throw "Could not open file, likely does not exist";
}

void Debug::Logger::log(std::string message)
{
    if (outputFileStream->good())
    {
        std::time_t t = std::time(0);
        std::tm now = {};
        localtime_s(&now, &t);

        (*outputFileStream)
            << "["
            << (now.tm_year + 1900) << '/'
            << (now.tm_mon > 8 ? //Ensures constant spacing, range: 0, 11
                std::to_string(now.tm_mon + 1) :
                std::to_string(0).append(std::to_string(now.tm_mon + 1))) << '/'
            << (now.tm_mday > 9 ? //Ensures constant spacing, range: 1, 31 (DO NOT ASK ME WHY THEY DECIDED THAT IT SHOULD BE 0 INDEXED FOR THE MONTH BUT NOT DAY?!?!?!)
                std::to_string(now.tm_mday) :
                std::to_string(0).append(std::to_string(now.tm_mday))) << "-"
            << now.tm_hour << ":"
            << now.tm_min << ":"
            << now.tm_sec << "]  "
            << message
            << std::endl;
    }
    else
    {
        throw "Stream is inaccessible";
    }
}

void Debug::Logger::log(std::string title, std::string message)
{
    if (outputFileStream->good())
    {
        std::time_t t = std::time(0);
        std::tm now = {};
        localtime_s(&now, &t);

        (*outputFileStream)
            << "["
            << (now.tm_year + 1900) << '/'
            << (now.tm_mon > 8 ? //Ensures constant spacing, range: 0, 11
                std::to_string(now.tm_mon + 1) :
                std::to_string(0).append(std::to_string(now.tm_mon + 1))) << '/'
            << (now.tm_mday > 9 ? //Ensures constant spacing, range: 1, 31 (DO NOT ASK ME WHY THEY DECIDED THAT IT SHOULD BE 0 INDEXED FOR THE MONTH BUT NOT DAY?!?!?!)
                std::to_string(now.tm_mday) :
                std::to_string(0).append(std::to_string(now.tm_mday))) << "-"
            << now.tm_hour << ":"
            << now.tm_min << ":"
            << now.tm_sec << "]  "
            << title << ": "
            << message
            << std::endl;
    }
    else
    {
        throw "Stream is inaccessible";
    }
}

bool Debug::Logger::isvalid()
{
    return outputFileStream->good();
}