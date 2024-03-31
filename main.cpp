#include <bits/stdc++.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "Game.h"

using namespace std;

Game* game = nullptr;

int main(int argc, char* argv[]){
    game = new Game();

    game->init();
    
    while(game->running()){
        game->run();
    }

    game->close();

    return 0;
}
