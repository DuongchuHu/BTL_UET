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