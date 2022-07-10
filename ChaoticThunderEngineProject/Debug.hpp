#ifndef DEBUG_LOGGING_MODULE
#define DEBUG_LOGGING_MODULE
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <stdarg.h>
#include <glad\glad.h>

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
        WARNING = 128,
        OPENGL = 256
    };

    enum class OpenGLError {
        NONE = GL_NO_ERROR,
        INVALID_ENUM = GL_INVALID_ENUM,
        INVALID_VALUE = GL_INVALID_VALUE,
        INVALID_OPERATION = GL_INVALID_OPERATION,
        STACK_OVERFLOW = GL_STACK_OVERFLOW,
        STACK_UNDERFLOW = GL_STACK_UNDERFLOW,
        OUT_OF_MEMORY = GL_OUT_OF_MEMORY,
        INVALID_FRAMEBUFFER_OPERATION = GL_INVALID_FRAMEBUFFER_OPERATION
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
        static void ConsoleOpenGLError(std::string message, bool terminate_if_error = true);
        void Log(Level level, std::string format, ...);
        void LogOpenGLError(std::string message, bool terminate_if_error = true);
        bool IsValid();
    };
}

#endif