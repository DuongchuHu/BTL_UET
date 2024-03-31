#ifndef GAME_H
#define GAME_H

#include <bits/stdc++.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

using namespace std;

class Game
{
public:
    Game();
    ~Game();

    void run();
    bool init();
    void close();
    bool running() { return isRunning;};
    
    static SDL_Renderer* renderer;
private:
    bool isRunning = true;
    SDL_Window* window;
};

#endif
