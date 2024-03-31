#ifndef TextureManager_HPP
#define TextureManager_HPP
#include "TextureManager.hpp"

SDL_Texture *TextureManager::Loadtexture(const char *texture)
{
    SDL_Surface* tmpSurface = IMG_Load(texture);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);

    return tex;
}
#endif