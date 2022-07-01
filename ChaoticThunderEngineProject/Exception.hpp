#ifndef UTILITY_EXCEPTION
#define UTILITY_EXCEPTION

#include <stdexcept>
#include <string.h>

class Exception : public std::exception {
private:
    std::string message;
public:
    Exception* SetMessage(std::string msg);

    virtual const char* what() const throw();
};

#endif