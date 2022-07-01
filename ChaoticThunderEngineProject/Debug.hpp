#ifndef DEBUG_LOGGING_MODULE
#define DEBUG_LOGGING_MODULE
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <stdarg.h>
namespace Debug
{
    enum class Level {
        CONSTRUCTION = 1,
        DESTRUCTION = 2,
        COPY = 4,
        WRITING = 8,
        READING = 16,
        CONTEXT = 32,
        INFO = 64,
        WARNING = 128
    };

    static std::ostream* defaultoutstream = &std::cout;
    static int loglevels = (int)Level::INFO | (int)Level::WARNING;

    class Logger
    {
    private:
        std::string _filename;
        std::ostream* _outputFileStream;
        int _loglevels = loglevels;
        static std::string levelToString(Level level);
    public:
        Logger(std::ostream& stream);
        ~Logger();

        static void Console(Level level, std::string format, ...);
        void Log(Level level, std::string format, ...);
        bool IsValid();
    };
}

#endif