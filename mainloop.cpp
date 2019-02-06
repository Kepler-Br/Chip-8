#include "mainloop.h"
#include <regex>

void MainLoop::initSdl()
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(
                "Chip 8 emulator.",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                windowWidth,
                windowHeight,
                SDL_WINDOW_RESIZABLE
                );

    if (window == nullptr) {
        throw std::runtime_error(SDL_GetError());
    }

    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
    if ( renderer == nullptr )
    {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_SetRenderDrawColor(renderer, std::get<0>(backgroundColor),
                                     std::get<1>(backgroundColor),
                                     std::get<2>(backgroundColor),
                                     255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

}

void MainLoop::render()
{
    const int CHIP_SCREEN_WIDTH = 64;
    const int CHIP_SCREEN_HEIGHT = 32;

    SDL_SetRenderDrawColor(renderer, std::get<0>(backgroundColor),
                                     std::get<1>(backgroundColor),
                                     std::get<2>(backgroundColor),
                                     255);
    SDL_RenderClear(renderer);
    for(int y = 0; y < CHIP_SCREEN_HEIGHT; y++)
    {
        for(int x = 0; x < CHIP_SCREEN_WIDTH; x++)
        {
            if(chipeight.getPixel(x, y) == 1)
            {
                SDL_Rect r;
                int width_coef = (windowWidth-CHIP_SCREEN_WIDTH)/CHIP_SCREEN_WIDTH+1;
                int height_coef = (windowHeight-CHIP_SCREEN_HEIGHT)/CHIP_SCREEN_HEIGHT+1;
                r.x = x*width_coef;
                r.y = y*height_coef;
                r.w = width_coef;
                r.h = height_coef;
                SDL_SetRenderDrawColor(renderer, std::get<0>(pixelColor),
                                                 std::get<1>(pixelColor),
                                                 std::get<2>(pixelColor),
                                                 255);
                SDL_RenderFillRect( renderer, &r );
            }
        }
    }
    chipeight.drawFlag = false;
    SDL_RenderPresent(renderer);
}

void MainLoop::processEvents()
{
    SDL_Event event;
    while ( SDL_PollEvent( &event ) )
    {
        if ( event.type == SDL_QUIT )
            running = false;
        else if ( event.type == SDL_KEYDOWN )
        {
            switch ( event.key.keysym.sym )
            {
            case SDLK_1:
                chipeight.pressKey(0x1);
                break;
            case SDLK_2:
                chipeight.pressKey(0x2);
                break;
            case SDLK_3:
                chipeight.pressKey(0x3);
                break;
            case SDLK_4:
                chipeight.pressKey(0xC);
                break;

            case SDLK_q:
                chipeight.pressKey(0x4);
                break;
            case SDLK_w:
                chipeight.pressKey(0x5);
                break;
            case SDLK_e:
                chipeight.pressKey(0x6);
                break;
            case SDLK_r:
                chipeight.pressKey(0xD);
                break;

            case SDLK_a:
                chipeight.pressKey(0x7);
                break;
            case SDLK_s:
                chipeight.pressKey(0x8);
                break;
            case SDLK_d:
                chipeight.pressKey(0x9);
                break;
            case SDLK_f:
                chipeight.pressKey(0xE);
                break;

            case SDLK_z:
                chipeight.pressKey(0xA);
                break;
            case SDLK_x:
                chipeight.pressKey(0x0);
                break;
            case SDLK_c:
                chipeight.pressKey(0xB);
                break;
            case SDLK_v:
                chipeight.pressKey(0xF);
                break;
            default :
                break;
            }
        }
        else if ( event.type == SDL_KEYUP )
        {
            switch ( event.key.keysym.sym )
            {
            case SDLK_1:
                chipeight.releaseKey(0x1);
                break;
            case SDLK_2:
                chipeight.releaseKey(0x2);
                break;
            case SDLK_3:
                chipeight.releaseKey(0x3);
                break;
            case SDLK_4:
                chipeight.releaseKey(0xC);
                break;

            case SDLK_q:
                chipeight.releaseKey(0x4);
                break;
            case SDLK_w:
                chipeight.releaseKey(0x5);
                break;
            case SDLK_e:
                chipeight.releaseKey(0x6);
                break;
            case SDLK_r:
                chipeight.releaseKey(0xD);
                break;

            case SDLK_a:
                chipeight.releaseKey(0x7);
                break;
            case SDLK_s:
                chipeight.releaseKey(0x8);
                break;
            case SDLK_d:
                chipeight.releaseKey(0x9);
                break;
            case SDLK_f:
                chipeight.releaseKey(0xE);
                break;

            case SDLK_z:
                chipeight.releaseKey(0xA);
                break;
            case SDLK_x:
                chipeight.releaseKey(0x0);
                break;
            case SDLK_c:
                chipeight.releaseKey(0xB);
                break;
            case SDLK_v:
                chipeight.releaseKey(0xF);
                break;
            default :
                break;
            }
        }
        if (event.type == SDL_WINDOWEVENT) {
            switch (event.window.event) {
            case SDL_WINDOWEVENT_RESIZED:
                windowWidth = event.window.data1;
                windowHeight = event.window.data2;
                render();
                break;
            }
        }
    }
}

void MainLoop::readConfig()
{
    std::ifstream configFile("./config.conf");
    if(configFile.fail())
    {
        configFile.close();
        std::cout << "Cannot read config.conf. Here is example of config file:\n";
        std::cout << "background_color = (0, 255, 0)\npixel_color = (255, 255, 255)\nwindow_geometry = (800, 600)\n";
        std::cout << "I'll try to create configuration file.\n";
        std::ofstream confFile("./config.conf");
        confFile << "background_color = (0, 255, 0)\npixel_color = (255, 255, 255)\nwindow_geometry = (800, 600)\n";
        confFile.close();
        return;
    }
    std::string config;
    for(std::string line = ""; getline(configFile, line); )
        config += "\n" + line;
    configFile.close();

    std::smatch match;

    std::regex backgroundColorRegex(R"(background_color[ ]*=[ ]*\((\d{1,3}),[ ]*(\d{1,3}),[ ]*(\d{1,3})\))");
    if(std::regex_search(config, match, backgroundColorRegex))
    {
        uint8_t r = static_cast<uint8_t>(std::stoi(match.str(1)));
        uint8_t g = static_cast<uint8_t>(std::stoi(match.str(2)));
        uint8_t b = static_cast<uint8_t>(std::stoi(match.str(3)));
        std::cout << "Set background_color to (" << int(r) << ", " << int(g) << ", " << int(b) << ").\n";
        backgroundColor = std::make_tuple(r, g, b);
    }
    else
    {
        std::cout << "No background_color property found in config.conf. Set to (0, 0, 0)\n";
    }

    std::regex pixelColorRegex(R"(pixel_color[ ]*=[ ]*\((\d{1,3}),[ ]*(\d{1,3}),[ ]*(\d{1,3})\))");
    if(std::regex_search(config, match, pixelColorRegex))
    {
        uint8_t r = static_cast<uint8_t>(std::stoi(match.str(1)));
        uint8_t g = static_cast<uint8_t>(std::stoi(match.str(2)));
        uint8_t b = static_cast<uint8_t>(std::stoi(match.str(3)));
        std::cout << "Set pixel_color to (" << int(r) << ", " << int(g) << ", " << int(b) << ").\n";
        pixelColor = std::make_tuple(r, g, b);
    }
    else
    {
        std::cout << "No pixel_color property found in config.conf. Set to (255, 255, 255)\n";
    }

    std::regex windowGeometryRegex(R"(window_geometry[ ]*=[ ]*\((\d{1,3}),[ ]*(\d{1,3}))");
    if(std::regex_search(config, match, windowGeometryRegex))
    {
        const int MINIMUM_WINDOW_SIZE = 25;
        int w = std::stoi(match.str(1));
        w = w < MINIMUM_WINDOW_SIZE? MINIMUM_WINDOW_SIZE:w;
        int h = std::stoi(match.str(2));
        h = h < MINIMUM_WINDOW_SIZE? MINIMUM_WINDOW_SIZE:h;
        std::cout << "Set window_geometry to (" << w << ", " << h << ").\n";
        windowHeight = h;
        windowWidth = w;
    }
    else
    {
        std::cout << "No window_geometry property found in config.conf. Set to (800, 600)\n";
    }
}

MainLoop::MainLoop(int argc, char *argv[]):
    backgroundColor(0, 0, 0),
    pixelColor(255, 255, 255)
{
    readConfig();
    initSdl();
    chipeight.init();
    chipeight.load(argv[1]);
    running = false;
}

MainLoop::~MainLoop()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void MainLoop::run()
{
    double f_deltaTime = 0;
    Uint32 currentTime = SDL_GetTicks();
    Uint32 lastTime = 0;
    Uint32 time = 0; // Используется для ограничения FPS

    running = true;
    while(running)
    {
        if (currentTime > lastTime)
            lastTime = currentTime;
        currentTime = SDL_GetTicks();
        f_deltaTime = (double)((currentTime - lastTime)/1000.0f);
        time = SDL_GetTicks();

        if(chipeight.drawFlag)
            render();
        processEvents();
        for(int i = 0; i < 20; i++)
        {
//            chipeight.printDebug();
            chipeight.cycle();
        }

        double n_FPScap = 30.0;

        if(1000.0/n_FPScap > SDL_GetTicks()-time)
        {
            SDL_Delay(1000.0/n_FPScap-(double)(SDL_GetTicks()-time));
        }
    }
}
