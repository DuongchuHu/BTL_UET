#include "TextureManager.hpp"

SDL_Texture *TextureManager::Loadtexture(const char *texture)
{
    SDL_Surface* tmpSurface = IMG_Load(texture);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);

    return tex;
}
SDL_Texture* TextureManager::LoadFontTexture(const char* text, int size, const char* Choose_font, const char* color){
    TTF_Font* Font = TTF_OpenFont(Choose_font, size);
    SDL_Color textColor;
    if (strcmp(color, "white") == 0) {
        textColor = {255,255,255};
    } else if (strcmp(color, "red") == 0) {
        textColor = {255,0,0};
    } else if (strcmp(color, "yellow") == 0) {
        textColor = {255,255,0};
    } else if (strcmp(color, "black") == 0) {
        textColor = {0,0,0};
    }
    SDL_Surface* textSurface = TTF_RenderText_Solid(Font, text, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(Game::renderer, textSurface);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(Font);
    return textTexture;
}
