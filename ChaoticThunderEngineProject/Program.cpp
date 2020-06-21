#include <iostream>
#include "exampleHeader.h"
#include "debug.h"

int main() {

    myPrintHelloMake();

    // call a function in another file
    std::cout << "Hello other world!" << std::endl;

    Debug::Logger logger(std::cout);

    logger.log("Hello from the log!");

    logger.log("Hello from the log! 2");

    logger.log("Im a title", "Hello from the log! 3");

    return(0);
}