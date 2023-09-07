#include "Exception.hpp"

Exception* Exception::SetMessage(std::string msg) {
	this->message = msg;
	return this;
}

const char* Exception::what() const throw()
{
	return message.c_str();
}