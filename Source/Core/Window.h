#pragma once

#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "../../include/SDL2/SDL.h"
#include "../../include/SDL2/SDL_ttf.h"
#include "../../include/SDL2/SDL_mixer.h"
#include <string>
#include <vector>
#include "PlayStateManager.h"
#include "../Menu/Button.h"

class Window {
    private:
        SDL_Window* window = nullptr; // cửa sổ game
        SDL_Renderer* renderer = nullptr; // màn hình game
        Menu* startMenu = nullptr; //con trỏ điều khiển menu
        // Menu* characterMenu = nullptr;
        PlayStateManager* playState = nullptr;

        LogStatus* Console = new LogStatus("Window");
        std::vector<std::string> highScore;
        bool Running = false;
        bool runningMenu = false;
        // bool runningCharacterMenu = false;
    public:
        static const int SCREEN_WIDTH = 882;
        static const int SCREEN_HEIGHT = 496;
        const std::string WINDOW_TITTLE = "Pacman";
        std::vector<std::string> startMenuButtonText = {"New Game", "How to Play", "High Scores", "Sound: ON", "Quit Game"};

        Window();

        ~Window();

        void initSDL();

        void quitSDL();

        void runGame();

};

#endif // _WINDOW_H_
