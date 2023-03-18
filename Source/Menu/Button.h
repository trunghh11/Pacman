#pragma once

#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "../Manager/TextManager.h"

class Button {
    private:
        SDL_Rect buttonRect;
        SDL_Rect buttonOutLineRect;
        TextManager* normalText;
        TextManager* selectText;
        TextManager* selectTextDetail;
        int buttonStatus;
        std::string bText;
        std::string bDetail;
    public:
        static const int BUTTON_OUT = 0;
        static const int BUTTON_IN  = 1;
        static const int BUTTON_PRESSED = 2;
        static const int MENU_FONT_SIZE = 28;
        const SDL_Color normalColor = {0, 0, 0, 255};
        const SDL_Color selectColor = {255, 255, 255, 255};
        const SDL_Color detailColor = {247, 247, 247, 255};

        Button();
        
        Button(int Width, int Height, int scrPosX, int scrPosY);

        void setButtonOutLine(int scrPosX, int scrPosY, int Width, int Height);

        SDL_Rect getButtonOutLine ();

        void loadButton(SDL_Renderer* &renderer, std::string text = "");

        SDL_Texture* loadButtonImage(SDL_Renderer* &renderer, const std::string imgPath);

        void renderButton(SDL_Renderer* &renderer,SDL_Texture* buttonTexture);

        void renderButOutLine(SDL_Renderer* &renderer, SDL_Rect buttonOutLineRect, int thickness);

        void setStatus(const int status);

        void changeSoundButton(SDL_Renderer* &renderer);

        bool checkMousePos(const int &x, const int &y) const;

        std::string getText() const;
};

#endif // _BUTTON_H
