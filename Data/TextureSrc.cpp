#include "../Source/Object/TextureSrc.h"
#include "../Source/Menu/Menu.h"

TextureSrc::TextureSrc() {
    blueTileTexture = nullptr;
    pinkTileTexture = nullptr;
    PacmanTexture = nullptr;
    MsPacmanTexture = nullptr;
    ghostScore = nullptr;
    pacmanFrame = 0;
    for (int i = 0; i < 7; ++i) ghostFrame[i] = 0;
}

TextureSrc::~TextureSrc() {
    SDL_DestroyTexture(blueTileTexture);
    blueTileTexture = nullptr;

    SDL_DestroyTexture(pinkTileTexture);
    pinkTileTexture = nullptr;

    SDL_DestroyTexture(PacmanTexture);
    PacmanTexture = nullptr;

    SDL_DestroyTexture(MsPacmanTexture);
    MsPacmanTexture = nullptr;

    SDL_DestroyTexture(ghostScore);
    ghostScore = nullptr;

    for (int i = 0; i < 7; ++i) ghostFrame[i] = 0;
}

bool TextureSrc::pacmanIsDead() {
    if (pacmanFrame == 109) {
        pacmanFrame = 0; return true;
    }
    return false;
}

void TextureSrc::loadTileTexture(SDL_Renderer* &renderer) {
    SDL_Surface* ImageBlue = nullptr;
    SDL_Surface* ImagePink = nullptr;

    ImagePink = IMG_Load("Source/Assets/Entity Image/Pacman Tile Labyrinth-pink.png");
    ImageBlue = IMG_Load("Source/Assets/Entity Image/Pacman Tile Labyrinth-blue.png");

    if (ImagePink == nullptr || ImageBlue == nullptr) {
        Console->Status( IMG_GetError() );
    }
    else {
        blueTileTexture = SDL_CreateTextureFromSurface(renderer, ImageBlue);
        pinkTileTexture = SDL_CreateTextureFromSurface(renderer, ImagePink);
        

        int x = 0, y = 0;
        for (int i = 0; i < 32; ++i) {
            tileSprite[i] = {x, y, 16, 16};
            y += 17;
            if (i % 4 == 3) x += 17, y = 0;
        }

        Console->Status("Tile Texture got successfully!");
    }

    SDL_FreeSurface(ImageBlue);
    SDL_FreeSurface(ImagePink);
    ImageBlue = nullptr;
    ImagePink = nullptr;
}

void TextureSrc::renderTileTexture(SDL_Renderer* &renderer, int tileID, SDL_Rect* dsRect, int &selectedCharacter) {
    if (selectedCharacter == Menu::MS_PAC_MAN) {
        SDL_RenderCopy(renderer, pinkTileTexture, &tileSprite[ tileID ], dsRect);
    }
    if (selectedCharacter == Menu::PAC_MAN) {
        SDL_RenderCopy(renderer, blueTileTexture, &tileSprite[ tileID ], dsRect);
    }

}

void TextureSrc::loadPacmanAndGhostTexture(SDL_Renderer* &renderer) {
    SDL_Surface* ImagePacman = nullptr;
    SDL_Surface* ImageMsPacman = nullptr;
    SDL_Surface* Image = nullptr;

    ImageMsPacman = IMG_Load("Source/Assets/Entity Image/MsPacman and Ghost Texture.png");
    ImagePacman = IMG_Load("Source/Assets/Entity Image/Pacman and Ghost Texture.png");

    if (ImagePacman == nullptr || ImageMsPacman == nullptr) {
        Console->Status( IMG_GetError() );
    }
    else {
        PacmanTexture = SDL_CreateTextureFromSurface(renderer, ImagePacman);
        MsPacmanTexture = SDL_CreateTextureFromSurface(renderer, ImageMsPacman);

        int posTexX = 0, posTexY = 0;

        /// Pacman goes up
        for (int i = 0; i < 3; ++i)  pacmanUP[i] = {posTexX, posTexY, 30, 30}, posTexX += 31;
        /// Pacman goes down
        for (int i = 3; i < 6; ++i)  pacmanDOWN[i % 3] = {posTexX, posTexY, 30, 30}, posTexX += 31;
        /// Pacman goes left
        for (int i = 6; i < 9; ++i)  pacmanLEFT[i % 3] = {posTexX, posTexY, 30, 30}, posTexX += 31;
        /// Pacman goes right
        for (int i = 9; i < 12; ++i) pacmanRIGHT[i % 3] = {posTexX, posTexY, 30, 30}, posTexX += 31;
        /// Pacman dead
        posTexX = 0;
        for (int i = 0; i < 11; ++i) pacmanDEAD[i] = {posTexX, 155, 30, 30}, posTexX += 31;

        Console->Status("Pacman Texture got successfully!");

        /// Ghost setup
        posTexX = 0; posTexY = 31;
        for (int i = 0; i < TOTAL_GHOST - 1; ++i) {
            ghost[i][UP   ][0] = {posTexX, posTexY, 30, 30}; posTexX += 31;
            ghost[i][UP   ][1] = {posTexX, posTexY, 30, 30}; posTexX += 31;
            ghost[i][DOWN ][0] = {posTexX, posTexY, 30, 30}; posTexX += 31;
            ghost[i][DOWN ][1] = {posTexX, posTexY, 30, 30}; posTexX += 31;
            ghost[i][LEFT ][0] = {posTexX, posTexY, 30, 30}; posTexX += 31;
            ghost[i][LEFT ][1] = {posTexX, posTexY, 30, 30}; posTexX += 31;
            ghost[i][RIGHT][0] = {posTexX, posTexY, 30, 30}; posTexX += 31;
            ghost[i][RIGHT][1] = {posTexX, posTexY, 30, 30}; posTexX += 31;
            ghost[i][FRIGHTEN_GHOST_1][0] = {posTexX, 31, 30, 30}; posTexX += 31;
            ghost[i][FRIGHTEN_GHOST_1][1] = {posTexX, 31, 30, 30}; posTexX += 31;
            ghost[i][FRIGHTEN_GHOST_2][0] = {posTexX, 31, 30, 30}; posTexX += 31;
            ghost[i][FRIGHTEN_GHOST_2][1] = {posTexX, 31, 30, 30}; posTexX = 0;
            posTexY += 31;
            if (i == 3) posTexY += 31;
        }
        posTexX = 248; posTexY = 62;
        ghost[GHOST_SPIRIT][UP   ][0] = {posTexX, posTexY, 30, 30};
        ghost[GHOST_SPIRIT][UP   ][1] = {posTexX, posTexY, 30, 30}; posTexX += 31;
        ghost[GHOST_SPIRIT][DOWN ][0] = {posTexX, posTexY, 30, 30};
        ghost[GHOST_SPIRIT][DOWN ][1] = {posTexX, posTexY, 30, 30}; posTexX = 248; posTexY += 31;
        ghost[GHOST_SPIRIT][LEFT ][0] = {posTexX, posTexY, 30, 30};
        ghost[GHOST_SPIRIT][LEFT ][1] = {posTexX, posTexY, 30, 30}; posTexX += 31;
        ghost[GHOST_SPIRIT][RIGHT][0] = {posTexX, posTexY, 30, 30};
        ghost[GHOST_SPIRIT][RIGHT][1] = {posTexX, posTexY, 30, 30}; posTexX = posTexY = 0;

        Image = IMG_Load("Source/Assets/Entity Image/ghostscore.png");
        ghostScore = SDL_CreateTextureFromSurface(renderer, Image);

        Console->Status("Ghost Texture got successfully!");
    }

    SDL_FreeSurface(ImagePacman);
    SDL_FreeSurface(ImageMsPacman);
    ImagePacman = nullptr;
    ImageMsPacman = nullptr;
    Image = nullptr;
}

void TextureSrc::renderPacmanTexture(SDL_Renderer* &renderer, int posX, int posY, int status, int &selectedCharacter) {
    SDL_Rect srcRect, dsRect;
    dsRect = {posX - 7 + 217, posY - 7, 30, 30};
    ++pacmanFrame;

    if (status != DEAD_PACMAN && pacmanFrame == 30) pacmanFrame = 0;

    switch (status) {
        case -1: srcRect = pacmanRIGHT[0]; break;
        case UP:    srcRect = pacmanUP   [ pacmanFrame / 10 ]; break;
        case RIGHT: srcRect = pacmanRIGHT[ pacmanFrame / 10 ]; break;
        case DOWN:  srcRect = pacmanDOWN [ pacmanFrame / 10 ]; break;
        case LEFT:  srcRect = pacmanLEFT [ pacmanFrame / 10 ]; break;
        case DEAD_PACMAN: srcRect = pacmanDEAD[pacmanFrame / 10]; break;
    }
    if (selectedCharacter == Menu::PAC_MAN) {
        SDL_RenderCopy(renderer, PacmanTexture, &srcRect, &dsRect);
    }
    if (selectedCharacter == Menu::MS_PAC_MAN) {
        SDL_RenderCopy(renderer, MsPacmanTexture, &srcRect, &dsRect);
    }
}

void TextureSrc::renderGhostTexture(SDL_Renderer* &renderer, int posX, int posY, int ghostID, int status) {
    SDL_Rect srcRect, dsRect;
    dsRect = {posX - 7 + 217, posY - 7, 30, 30};
    ++ghostFrame[ghostID];

    if (ghostFrame[ghostID] == 14) ghostFrame[ghostID] = 0;

    switch (status) {
        case UP:    srcRect = ghost[ ghostID ][UP   ][ ghostFrame[ghostID] / 7 ]; break;
        case RIGHT: srcRect = ghost[ ghostID ][RIGHT][ ghostFrame[ghostID] / 7 ]; break;
        case DOWN:  srcRect = ghost[ ghostID ][DOWN ][ ghostFrame[ghostID] / 7 ]; break;
        case LEFT:  srcRect = ghost[ ghostID ][LEFT ][ ghostFrame[ghostID] / 7 ]; break;
        case FRIGHTEN_GHOST_1: srcRect = ghost[ghostID][FRIGHTEN_GHOST_1][ghostFrame[ghostID] / 7]; break;
        case FRIGHTEN_GHOST_2:
            if (ghostFrame[ghostID] / 7 == 0) srcRect = ghost[ghostID][FRIGHTEN_GHOST_1][ghostFrame[ghostID] / 7];
            else srcRect = ghost[ghostID][FRIGHTEN_GHOST_2][ghostFrame[ghostID] / 7];
            break;
    }

    SDL_RenderCopy(renderer, PacmanTexture, &srcRect, &dsRect);
}

void TextureSrc::renderGhostScore(SDL_Renderer* &renderer, const int eatenGhostPosX, const int eatenGhostPosY, const int eatenGhostStreak) {
    SDL_Rect srcRect, dsRect;
    switch (eatenGhostStreak) {
        case 0: srcRect = { 0,  0, 50, 50}; break;
        case 1: srcRect = {50,  0, 50, 50}; break;
        case 2: srcRect = { 0, 50, 50, 50}; break;
        case 3: srcRect = {50, 50, 50, 50}; break;
    }
    dsRect = {eatenGhostPosX + 210, eatenGhostPosY - 7, 30, 30};
    SDL_RenderCopy(renderer, ghostScore, &srcRect, &dsRect);
}
