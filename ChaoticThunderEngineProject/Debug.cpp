#include "Debug.hpp"
#include <glad\glad.h>

namespace Debug {

    void SetLogLevels(int newlevels) {
        loglevels = newlevels;
    }

    std::string Logger::levelToString(Level level) {
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
        case Level::OPENGL:
            return "[OPENGL]   ";
        default:
            return "";
        };
    }

    Logger::Logger(std::ostream& stream) {
        if (stream.good())
            _outputFileStream = &stream;
        else {
            std::exception ex("Stream is inaccessible (not good)");
            throw ex;
        }
    }

    Logger::~Logger() {
        _outputFileStream->flush();
        //delete _outputFileStream;
    }

    void Logger::Console(Level level, std::string format, ...)
    {
        int result = ((int)level) & loglevels;
        if (result == 0)
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

    void Logger::ConsoleOpenGLError(std::string message, bool terminate_if_error) {
        int error = glGetError();
        switch ((OpenGLError)error) {
        case OpenGLError::NONE:
            return;
        case OpenGLError::INVALID_ENUM:
            Console(Level::OPENGL, "Invalid enumeration parameter: " + message);
            break;
        case OpenGLError::INVALID_VALUE:
            Console(Level::OPENGL, "Invalid value parameter: " + message);
            break;
        case OpenGLError::INVALID_OPERATION:
            Console(Level::OPENGL, "Invalid command, not legal for its given set of parameters: " + message);
            break;
        case OpenGLError::STACK_OVERFLOW:
            Console(Level::OPENGL, "Stack overflow: " + message);
            break;
        case OpenGLError::STACK_UNDERFLOW:
            Console(Level::OPENGL, "Stack underflow: " + message);
            break;
        case OpenGLError::OUT_OF_MEMORY:
            Console(Level::OPENGL, "Out of memory: " + message);
            break;
        case OpenGLError::INVALID_FRAMEBUFFER_OPERATION:
            Console(Level::OPENGL, "Attempted read or write to incomplete framebuffer: " + message);
            break;
        }
        if (terminate_if_error > 0) {
            std::exception ex(message.c_str());
            throw ex;
        }
    }

    void Logger::Log(Level level, std::string format, ...) {
        int result = ((int)level) & loglevels;
        if (result == 0)
            return;

        if (_outputFileStream->good())
        {
            std::time_t t = std::time(0);
            std::tm now = {};
            localtime_s(&now, &t);

            (*_outputFileStream)
                << Logger::levelToString(level)
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
            std::exception ex("Stream is inaccessible");
            throw ex;
        }
    }

    void Logger::LogOpenGLError(std::string message, bool terminate_if_error) {
        int error = glGetError();
        switch ((OpenGLError)error) {
        case OpenGLError::NONE:
            return;
        case OpenGLError::INVALID_ENUM:
            Log(Level::OPENGL, "Invalid enumeration parameter: %s", message);
            break;
        case OpenGLError::INVALID_VALUE:
            Log(Level::OPENGL, "Invalid value parameter: %s", message);
            break;
        case OpenGLError::INVALID_OPERATION:
            Log(Level::OPENGL, "Invalid command, not legal for its given set of parameters: %s", message);
            break;
        case OpenGLError::STACK_OVERFLOW:
            Log(Level::OPENGL, "Stack overflow: %s", message);
            break;
        case OpenGLError::STACK_UNDERFLOW:
            Log(Level::OPENGL, "Stack underflow: %s", message);
            break;
        case OpenGLError::OUT_OF_MEMORY:
            Log(Level::OPENGL, "Out of memory: %s", message);
            break;
        case OpenGLError::INVALID_FRAMEBUFFER_OPERATION:
            Log(Level::OPENGL, "Attempted read or write to incomplete framebuffer: %s", message);
            break;
        }
        if (terminate_if_error > 0) {
            std::exception ex(message.c_str());
            throw ex;
        }
    }

    bool Logger::IsValid()
    {
        return _outputFileStream->good();
    }
}