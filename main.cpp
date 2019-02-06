#include <iostream>
#include "mainloop.h"

int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        std::cout << "No rom path provided.\n";
        return 0;
    }
    MainLoop mainLoop(argc, argv);
    mainLoop.run();
   
    return 0;
}
