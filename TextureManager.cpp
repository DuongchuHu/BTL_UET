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
SDL_Texture* TextureManager::LoadFontTexture(const char* text, int size){
    TTF_Font* Font = TTF_OpenFont("LIT-Mango.ttf", size);

    SDL_Color textColor = { 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(Font, text, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(Game::renderer, textSurface);
    SDL_FreeSurface(textSurface);

    return textTexture;
}
#endif#ifndef TextureManager_HPP
#define TextureManager_HPP
#include "TextureManager.hpp"

SDL_Texture *TextureManager::Loadtexture(const char *texture)
{
    SDL_Surface* tmpSurface = IMG_Load(texture);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);

    return tex;
}
SDL_Texture* TextureManager::LoadFontTexture(const char* text, int size){
    TTF_Font* Font = TTF_OpenFont("LIT-Mango.ttf", size);

    SDL_Color textColor = { 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(Font, text, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(Game::renderer, textSurface);
    SDL_FreeSurface(textSurface);

    return textTexture;
}
#endif