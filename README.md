# ChaoticThunderEngine
A project made to learn OpenCL and create possibly a physics engine

## Engine
This section will cover mainly what is stored in the /EngineProject

### Makefile explaination
We are using GCC for compiling to maximize the options we have in exchange for some added complexity to the compilation process

Firstly, to perform a simple comilation and run, you can use:
```
gcc -lstdc++ -Wall -Wextra -Werror Program.cpp -o Program.o  
```