#include "../Source/Menu/Button.h"

Button::Button(){}

Button::Button(int Width, int Height, int scrPosX, int scrPosY) {
    buttonRect = {scrPosX, scrPosY, Width, Height};
    normalText = new TextManager(24);
    selectText = new TextManager(25);
    selectTextDetail = new TextManager(18);
}

SDL_Texture* Button::loadButtonImage(SDL_Renderer* &renderer, const std::string imgPath) {
    SDL_Surface* Image = IMG_Load(imgPath.c_str());
    SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(renderer, Image);
    SDL_FreeSurface(Image);
    return buttonTexture;
}

void Button::setButtonOutLine(int scrPosX, int scrPosY, int Width, int Height) {
    buttonOutLineRect = {scrPosX, scrPosY, Width, Height}; //viền
}

SDL_Rect Button::getButtonOutLine() {
    return buttonOutLineRect;
}

void Button::loadButton(SDL_Renderer* &renderer, std::string text) {
    if (text == "") {
        delete selectText;
        selectText = nullptr;
        delete normalText;
        normalText = nullptr;
        delete selectTextDetail;
        selectTextDetail = nullptr;
        return;  
    } 
    normalText->loadRenderText(renderer, text, normalColor);
    selectText->loadRenderText(renderer, text, selectColor);
    bText = text;
    if (text == "New Game") bDetail = "Press Enter to play a new game.";
    else if (text == "Resume") bDetail = "Press Enter to continue the game.";
    else if (text == "How to Play") bDetail = "Press Enter for instructions on how to play.";
    else if (text == "High Scores") bDetail = "Press Enter for High score.";
    else if (text == "Sound: ON") bDetail = "Press Enter to change sound status.";
    else if (text == "Quit Game") bDetail = "Press Enter to quit the game.";
    else if (text == "Back to Menu") bDetail = "Press Enter to return to the Start Menu.";
    else bDetail = "";

    if (bDetail != "") selectTextDetail->loadRenderText(renderer, bDetail, detailColor);
    else {
        delete selectTextDetail;
        selectTextDetail = nullptr;
    }
}

void Button::renderButton(SDL_Renderer* &renderer, SDL_Texture* buttonTexture) {       
    if (buttonStatus == BUTTON_IN) {
        SDL_RenderCopy(renderer, buttonTexture, NULL, &buttonRect);
        if (selectText != nullptr) {
            SDL_SetRenderDrawColor(renderer, selectColor.r, selectColor.g, selectColor.b, selectColor.a);
            selectText->renderText(renderer, buttonRect.x + buttonRect.w / 2, buttonRect.y + buttonRect.h / 2, TextManager::CENTER);
        }

        if (selectTextDetail != nullptr) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            selectTextDetail->renderText(renderer, 350, 400, TextManager::CENTER);
        }
    }
    else if (buttonStatus == BUTTON_OUT) {
        if (normalText != nullptr) {
            SDL_SetRenderDrawColor(renderer, normalColor.r, normalColor.g, normalColor.b, normalColor.a);
            normalText->renderText(renderer, buttonRect.x + buttonRect.w / 2, buttonRect.y + buttonRect.h / 2, TextManager::CENTER);
        }
    }
}

void Button::renderButOutLine(SDL_Renderer* &renderer, SDL_Rect buttonOutLineRect, int thickness) {
    if (buttonStatus == BUTTON_IN) {
        //draw border
        SDL_Rect rect;
        for (int i = 0; i < thickness; i++) {
            rect ={buttonOutLineRect.x - i, buttonOutLineRect.y - i, buttonOutLineRect.w + i*2, buttonOutLineRect.h + i*2};
            SDL_SetRenderDrawColor(renderer, 252, 207, 0, 255);
            SDL_RenderDrawRect(renderer, &rect);
        }

    }
}

void Button::setStatus(const int status) {
    buttonStatus = status;
}

void Button::changeSoundButton(SDL_Renderer* &renderer) {
    if (bText == "Sound: ON") bText = "Sound: OFF";
    else bText = "Sound: ON";
    normalText->loadRenderText(renderer, bText, normalColor);
    selectText->loadRenderText(renderer, bText, selectColor);
    buttonStatus = BUTTON_IN;
}

bool Button::checkMousePos(const int &x, const int &y) const {
    if (x < buttonRect.x || x > buttonRect.x + buttonRect.w) return false;
    if (y < buttonRect.y || y > buttonRect.y + buttonRect.h) return false;
    return true;
}

std::string Button::getText() const {
    return bText;
}
