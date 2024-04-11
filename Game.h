#ifndef GAME_H
#define GAME_H

#include <bits/stdc++.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

class Game
{
private:
    SDL_Window* window;
    bool isRunning = true;
    Uint32 CountTime = 0;
    bool isMainScreen = true;
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
    void pushScore();

    static TTF_Font* GameOverFont;
    static SDL_Renderer* renderer;
};

#endif
