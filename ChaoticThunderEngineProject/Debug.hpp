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

    std::ostream* defaultoutstream = &std::cout;
    int loglevels = (int)Level::INFO | (int)Level::WARNING;

    class Logger
    {
    private:
        std::string _filename;
        std::ostream* _outputFileStream;
        int _loglevels = loglevels;
        static std::string levelToString(Level level) {
            switch (level)
            {
            case Level::CONSTRUCTION:
                return "[CONSTRUCT]";
            case Level::DESTRUCTION:
                return "[DESTRUCT] ";
            case Level::COPY:
                return "[COPY]     ";
            case Level::WRITING:
                return "[WRITE]    ";
            case Level::READING:
                return "[READ]     ";
            case Level::CONTEXT:
                return "[CONTEXT]  ";
            case Level::INFO:
                return "[INFO]     ";
            case Level::WARNING:
                return "[WARNING]  ";
            default:
                return "";
            };
        }
    public:
        Logger(std::ostream& stream) {
            if (stream.good())
                _outputFileStream = &stream;
            else
                throw "Could not open file, likely does not exist";
        }

        ~Logger() {
            _outputFileStream->flush();
        }

        static void Console(Level level, std::string format, ...)
        {
            if (((int)level & loglevels) == 0)
                return;

            std::time_t t = std::time(0);
            std::tm now = {};
            localtime_s(&now, &t);

            *defaultoutstream
                << levelToString(level)
                << "["
                << (now.tm_year + 1900) << '/'
                << (now.tm_mon > 8 ? //Ensures constant spacing, range: 0, 11
                    std::to_string(now.tm_mon + 1) :
                    std::to_string(0).append(std::to_string(now.tm_mon + 1))) << '/'
                << (now.tm_mday > 9 ? //Ensures constant spacing, range: 1, 31
                    std::to_string(now.tm_mday) :
                    std::to_string(0).append(std::to_string(now.tm_mday))) << "-"
                << now.tm_hour << ":"
                << now.tm_min << ":"
                << now.tm_sec << "]  ";

            va_list valist;
            va_start(valist, format);
            for (const char* str_ptr = format.c_str(); *str_ptr; str_ptr++) {
                int offset = 0;
                bool leftOrientated = false;
                std::string tempstring("\0");
                switch (*str_ptr)
                {
                    case '%':
                        str_ptr++;
                        if (*str_ptr == '-') {
                            leftOrientated = true;
                            str_ptr++;
                            if (!isdigit(*str_ptr))
                                throw std::runtime_error("Invalid formatting, expected number after minus '-'");
                        }

                        char buffer[11];
                        int i;
                        for (i = 0; i < 10; i++)
                            buffer[i] = '0';
                        buffer[10] = '\0';
                        for (i = 0; isdigit(*str_ptr) && i < 10; i++, str_ptr++)
                            buffer[i] = *str_ptr;
                        //11 -> 11000000000
                        for (int j = 10 - i; j < 10; j++) {
                            buffer[j] = buffer[j - (10 - i)];
                            buffer[j - (10 - i)] = '0';
                        }
                        if (i > 0)
                            offset = atoi(buffer);

                        switch (*str_ptr)
                        {
                            case 'd': //whole-numeric value
                                tempstring = std::to_string(va_arg(valist, long));
                                break;
                            case 'f': //floating point decimal value
                                tempstring = std::to_string(va_arg(valist, double));
                                tempstring = tempstring.substr(0, tempstring.find_last_not_of('0') + 1);
                                break;
                            case 's': //"string"
                                tempstring = std::string(va_arg(valist, char*));
                                break;
                            default:
                                throw std::runtime_error("Format not supproted");
                                break;
                        }

                        if (offset != 0) {
                            if (tempstring.size() >= offset)
                                *defaultoutstream << tempstring;
                            else {
                                if (leftOrientated) {
                                    *defaultoutstream << tempstring;
                                    for (i = (int)tempstring.size(); i <= offset; i++)
                                        *defaultoutstream << ' ';
                                }
                                else {
                                    for (i = (int)tempstring.size(); i <= offset; i++)
                                        *defaultoutstream << ' ';
                                    *defaultoutstream << tempstring;
                                }
                            }
                        }
                        else
                            *defaultoutstream << tempstring;
                        break;
                    default:
                        *defaultoutstream << *str_ptr;
                        break;
                }
            }
            va_end(valist);

            *defaultoutstream << std::endl;
        }

        void Log(Level level, std::string format, ...) {
            if (((int)level & loglevels) == 0)
                return;

            if (_outputFileStream->good())
            {
                std::time_t t = std::time(0);
                std::tm now = {};
                localtime_s(&now, &t);

                (*_outputFileStream)
                    << levelToString(level)
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
                    << now.tm_sec << "]  ";

                va_list valist;
                va_start(valist, format);
                for (const char* str_ptr = format.c_str(); *str_ptr; str_ptr++) {
                    int offset = 0;
                    bool leftOrientated = false;
                    std::string tempstring("\0");
                    switch (*str_ptr)
                    {
                    case '%':
                        str_ptr++;
                        if (*str_ptr == '-') {
                            leftOrientated = true;
                            str_ptr++;
                            if (!isdigit(*str_ptr))
                                throw std::runtime_error("Invalid formatting, expected number after minus '-'");
                        }

                        char buffer[11];
                        int i;
                        for (i = 0; i < 10; i++)
                            buffer[i] = '0';
                        buffer[10] = '\0';
                        for (i = 0; isdigit(*str_ptr) && i < 10; i++, str_ptr++)
                            buffer[i] = *str_ptr;
                        //11 -> 11000000000
                        for (int j = 10 - i; j < 10; j++) {
                            buffer[j] = buffer[j - (10 - i)];
                            buffer[j - (10 - i)] = '0';
                        }
                        if (i > 0)
                            offset = atoi(buffer);

                        switch (*str_ptr)
                        {
                        case 'd': //whole-numeric value
                            tempstring = std::to_string(va_arg(valist, long));
                            break;
                        case 'f': //floating point decimal value
                            tempstring = std::to_string(va_arg(valist, double));
                            tempstring = tempstring.substr(0, tempstring.find_last_not_of('0') + 1);
                            break;
                        case 's': //"string"
                            tempstring = va_arg(valist, std::string);
                            break;
                        default:
                            throw std::runtime_error("Format not supproted");
                            break;
                        }

                        if (offset != 0) {
                            if (tempstring.size() >= offset)
                                std::cout << tempstring;
                            else {
                                if (leftOrientated) {
                                    std::cout << tempstring;
                                    for (i = (int)tempstring.size(); i <= offset; i++)
                                        std::cout << ' ';
                                }
                                else {
                                    for (i = (int)tempstring.size(); i <= offset; i++)
                                        (*_outputFileStream) << ' ';
                                    (*_outputFileStream) << tempstring;
                                }
                            }
                        }
                        else
                            (*_outputFileStream) << tempstring;
                        break;
                    default:
                        (*_outputFileStream) << *str_ptr;
                        break;
                    }
                }
                va_end(valist);
                (*_outputFileStream) << std::endl;
                _outputFileStream->flush();
            }
            else
            {
                throw "Stream is inaccessible";
            }
        }

        bool IsValid()
        {
            return _outputFileStream->good();
        }
    };
}

#endif