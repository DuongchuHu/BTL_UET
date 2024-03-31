#include "Game.h"

using namespace std;

class TextureManager
{
public:
    TextureManager();
    ~TextureManager();
    static SDL_Texture* Loadtexture(const char* fileName);
};
