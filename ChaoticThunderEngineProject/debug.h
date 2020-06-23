#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <stdarg.h>
namespace Debug
{
    class Logger
    {
    private:
        std::string filename;
        std::ostream* outputFileStream;
    public:
        Logger(std::ostream& stream) {
            if (stream.good())
                outputFileStream = &stream;
            else
                throw "Could not open file, likely does not exist";
        }

        static void console(std::string format, ...)
        {
            std::time_t t = std::time(0);
            std::tm now = {};
            localtime_s(&now, &t);

            std::cout
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
                                tempstring = std::string(va_arg(valist, char*));
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
                                    for (i = tempstring.size(); i <= offset; i++)
                                        std::cout << ' ';
                                }
                                else {
                                    for (i = tempstring.size(); i <= offset; i++)
                                        std::cout << ' ';
                                    std::cout << tempstring;
                                }
                            }
                        }
                        else
                            std::cout << tempstring;
                        break;
                    default:
                        std::cout << *str_ptr;
                        break;
                }
            }
            va_end(valist);
            std::cout << std::endl;
        }

        void log(std::string format, ...)
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
                                    for (i = tempstring.size(); i <= offset; i++)
                                        std::cout << ' ';
                                }
                                else {
                                    for (i = tempstring.size(); i <= offset; i++)
                                        (*outputFileStream) << ' ';
                                    (*outputFileStream) << tempstring;
                                }
                            }
                        }
                        else
                            (*outputFileStream) << tempstring;
                        break;
                    default:
                        (*outputFileStream) << *str_ptr;
                        break;
                    }
                }
                va_end(valist);
                (*outputFileStream) << std::endl;
                outputFileStream->flush();
            }
            else
            {
                throw "Stream is inaccessible";
            }
        }

        bool isvalid()
        {
            return outputFileStream->good();
        }
    };
}