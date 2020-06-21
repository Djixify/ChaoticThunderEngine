#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
namespace Debug
{
    class Logger
    {
    private:
        std::string filename;
        std::ostream* outputFileStream;
    public:
        Logger(std::ostream& stream);

        void log(std::string message);

        void log(std::string title, std::string message);

        bool isvalid();
    };
}