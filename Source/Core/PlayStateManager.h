#pragma once

#ifndef _PLAYSTATEMANAGER_H_
#define _PLAYSTATEMANAGER_H_

#include <string>
#include <vector>
#include "Engine.h"
#include <SDL_mixer.h>
#include "../Menu/Button.h"
#include "../Menu/Menu.h"
#include "../Object/Object.h"

class PlayStateManager {
    private:
        std::vector<std::string> scoreData;
        std::vector<int> muteChannel;
        Engine* engine = nullptr;
        Menu* pauseMenu = nullptr;
        Menu* characterMenu = nullptr;
        // bool running = true;
        bool runPauseMenu = false;
        bool runningCharacterMenu = false;
    public:
        std::vector<std::string> pauseMenuButtonText = {"Resume", "New Game", "How to Play", "Sound: ON", "Back to Menu"};


        PlayStateManager();

        ~PlayStateManager();

        bool isRunningCharacter();

        void setRunningCharacter(bool status);

        void initCharacter(SDL_Renderer* &renderer,const std::string imgPath);

        void renderCharacter(SDL_Renderer* &renderer );

        void handleEvent(SDL_Event &e, SDL_Renderer* &renderer, bool &runningMain, std::vector<std::string> &scoreData);

        void runCharacterMenu(SDL_Renderer* &renderer, SDL_Event &e );

        void newGame(SDL_Renderer* &renderer);

        void runGame(bool &exitToMenu);

        void render(SDL_Renderer* &renderer, const std::vector<std::string> &scoreData);
};

#endif // _PLAYSTATEMANAGER_H_
