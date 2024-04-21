#ifndef GAME_H
#define GAME_H

#include <bits/stdc++.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "TextureManager.hpp"
// #include "Bullet.h"

using namespace std;

class Game
{
private:
    SDL_Window* window;
    bool isRunning = true;
    Uint32 CountTime = 0;
    bool isMainScreen = true;
    bool isPlayScreen = true;
    bool isScoreScreen = true;
    bool isGameModeScreen = true;
    int FPS = 144;
    int number = 5;
public:
    Game();
    ~Game();

    void run();
    bool init();
    void close();
    bool running() { return isRunning;};
    void printMainScreen();
    void printPlayScreen();
    void printScoreScreen();
    void printGameModeScreen();
    void pushScore();
    void SwitchScreen(int x);

    static TTF_Font* GameOverFont;
    static SDL_Renderer* renderer;
};

#endif
