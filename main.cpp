#include <iostream>
#include "chipeight.h"
#include <chrono>
#include <thread>
#include <mainloop.h>

void draw(ChipEight &eight)
{
    if(eight.drawFlag)
    {
        for(int y = 0; y < 32; y++)
        {
            for(int x = 0; x < 64; x++)
            {
                if(eight.getPixel(x, y) == 1)
                    std::cout << "█";
                else
                    std::cout << "░";
            }
            std::cout << "\n";
        }
        eight.drawFlag = false;
    }
}

void debugExe(ChipEight &eight)
{
    while(true)
    {
        eight.printDebug();
        eight.cycle();
        draw(eight);
        std::string dump;
        std::cin.get();
    }
}

void exec(ChipEight &eight)
{
    while(true)
    {
        eight.cycle();
        draw(eight);
        std::this_thread::sleep_for(std::chrono::microseconds(10000));
    }
}

int main(int argc, char *argv[])
{
    ChipEight eight;
    eight.init();

#ifdef QT_QML_DEBUG
//    eight.load("./spaceInvaders.ch8");
//    debugExe(eight);
    MainLoop mainLoop(argc, argv);
    mainLoop.run();
#endif // QT_QML_DEBUG

#ifdef RELEASE
    if(argc == 1)
    {
        std::cout << "No argument provided.\n";
        return 0;
    }
    eight.load(argv[1]);
    exec(eight);
#endif // RELEASE

   
    return 0;
}
