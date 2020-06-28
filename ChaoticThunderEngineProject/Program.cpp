#include <iostream>
#include <fstream>
#include <filesystem>
#include "exampleHeader.h"
#include "debug.h"
#include "fileUtility.h"

int main() {

    myPrintHelloMake();

    // call a function in another file
    std::cout << "Hello other world!" << std::endl;


    Debug::Logger::console("%10d ::: %-15d ::: %10f", 9001, 42, 23.2);
    Debug::Logger::console("%10d ::: %-15d ::: %10f", 69, 360, 1000.2);

    std::string currentPath = FileUtility::currentDirectory();
    std::string parent = FileUtility::getParent(currentPath);
    std::string firstuwu = FileUtility::combinePath(currentPath, "Somefile.uwu");

    std::cout << currentPath << std::endl;
    std::cout << parent << std::endl;
    std::cout << firstuwu << std::endl;

    Debug::Logger* tmp = new Debug::Logger(std::cout);

    //Below is equivalent, *pointer is dereference, &value is reference, type* referes to a pointer of the type 'type'
    //Dereference will take the value that the pointer points to
    //Reference will provide the address for the value 
    int a = 20;
    int* a_ptr = &a;
    int b = *a_ptr;
    printf("a=%d, addres of a: %p, addres of a_ptr: %p, b=%d, addres of b: %p\n", a, &a, &a_ptr, b, &b);
    
    tmp->log("Test");
    (*tmp).log("Test");

    std::ofstream filestream("Test.log"); //huh, no path required...
    Debug::Logger logger(filestream);

    logger.log("Hello from the log! 2");

    logger.log("Im a title", "Hello from the log! 3");

    logger.log("Im your execution path!: %s", FileUtility::currentDirectory());

    printf("%d\n", logger.isvalid());

    filestream.close();

    printf("%d\n", logger.isvalid());

    return(0);
}