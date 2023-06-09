#include "../Source/Core/Window.h"
#include "../Source/Manager/LogStatus.h"
#include "../Source/Core/PlayStateManager.h"
// #include "../Source/Manager/GameManager.h"
#include <math.h>

Window::Window() {
    window = nullptr;
    renderer = nullptr;
    playState = nullptr;
    startMenu = nullptr;
    // characterMenu = nullptr;
    runningMenu = false;
    // runningCharacterMenu = false;
}

Window::~Window() {
    delete playState;
    playState = nullptr;

    delete startMenu;
    startMenu = nullptr;

    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window != nullptr) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}

void Window::initSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        Console->Status( SDL_GetError() );
    }
    else {
        window = SDL_CreateWindow(WINDOW_TITTLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        Console->Status("Window opened successfully!");
        Running = true;

        if (window == nullptr) {
            Console->Status( SDL_GetError() );
        }
        else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            Console->Status("Renderer created successfully!");
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
            SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

            if (TTF_Init() < 0) Console->Status( TTF_GetError() );
            else Console->Status("TTF Ready!");

            if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 8, 2048) < 0) Console->Status( Mix_GetError() );
            else Console->Status("Audio Ready!");
            Mix_Volume(-1, MIX_MAX_VOLUME);

            freopen("score.txt", "r", stdin);
            std::string t = "";
            for (int i = 0; i < 10; ++i) {
                std::getline(std::cin, t);
                if (t.size()) {
                    highScore.push_back(t);
                }
                t = "";
                
            }
        }
    }
}

void Window::quitSDL() {
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
    Mix_Quit();
    TTF_Quit();
    atexit(SDL_Quit);
    Console->Status("Successfully exited!");
    freopen("score.txt", "w", stdout);
    for (int i = 0; i < fmin(10,highScore.size()); ++i) std::cout << highScore[i] << std::endl;
    return;
}

void Window::runGame() {
    startMenu = new Menu(236, 85, startMenuButtonText.size(), 220, 30);
    startMenu->init(renderer, "Source/Assets/Menu_Image/Pacman_Start_Menu.png", startMenuButtonText);
    startMenu->changeRunStatus();

    SDL_Event e;
    runningMenu = true;
    bool startGame = false;
    playState = new PlayStateManager();
    playState ->initCharacter(renderer, "Source/Assets/Menu_Image/character_menu.png");
    playState->setRunningCharacter(false);


    while (Running) {

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) Running = false;
            else {
                if (runningMenu) {
                    startMenu->handleEvent(e, renderer);
                    switch (startMenu->getStatus()) {
                        case Menu::PLAY_BUTTON_PRESSED:
                            runningMenu = false;
                            playState->setRunningCharacter(true);
                            // std::cout << playState->runningCharacterMenu << std::endl;
                            // std::cout << "pressed" << std::endl;
                            break;
                        case Menu::EXIT_BUTTON_PRESSED:
                            Running = false; break;
                    }
                }
                else if (playState->isRunningCharacter()) {
                    playState->runCharacterMenu(renderer, e);
                }
                else if (playState->isRunningCharacter() == false && runningMenu == false ){
                    
                    playState->handleEvent(e, renderer, runningMenu, highScore);
                    if (runningMenu) {
                        startMenu->reOpen();
                        playState->setRunningCharacter(false);
                        startGame = false;

                    } 
                }
            }
        }

        if (!runningMenu) {
            // if (startMenu->getStatus() == Menu::PLAY_BUTTON_PRESSED) {
            //     playState->newGame(renderer);
            // }
            if (playState->isRunningCharacter() == false && startGame == false) {
                playState -> newGame(renderer);
                startGame = true;
            }
            if (!playState->isRunningCharacter() ) {
                playState->runGame(runningMenu);
                if (runningMenu) startMenu->reOpen(),  playState->setRunningCharacter(false), startGame = false;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);


        if (runningMenu)
            startMenu->render(renderer, highScore);
        else if (playState->isRunningCharacter())
            playState->renderCharacter(renderer);
        else if (!playState->isRunningCharacter()) playState->render(renderer, highScore);

        SDL_RenderPresent(renderer);
    }
}
