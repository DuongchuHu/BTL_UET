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
    bool isPlaying = true;
    bool isMainScreen = true;
    bool isPlayScreen = true;
    bool isScoreScreen = true;
    bool isGameModeScreen = true;
    bool Pause = false;

    Uint32 CountTime = 0;
    Uint32 PauseTime = 0;
    int FPS = 120;
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
    void printGameOverScreen();
    void printPauseScreen();
    void pushScore();
    void SwitchScreen(int x);
    void SwitchMode(int Mode){
        switch (Mode)
        {
        case 1:
            FPS = 120;
            number = 5;
            break;
        case 2:
            FPS = 180;
            number = 7;
            break;
        case 3:
            FPS = 210;
            number = 9;
            break;
        default:
            break;
        }
    }

    static TTF_Font* GameOverFont;
    static SDL_Renderer* renderer;
};

#endif
