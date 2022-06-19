#ifndef UTILITY_EXCEPTION
#define UTILITY_EXCEPTION

#include <stdexcept>
#include <string.h>

class Exception : public std::exception {
private:
    std::string message;
public:
    Exception* SetMessage(std::string msg) {
        this->message = msg;
        return this;
    }

    virtual const char* what() const throw()
    {
        return message.c_str();
    }
};

#endif