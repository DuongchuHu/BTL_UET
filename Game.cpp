#include "Game.h"
#include "TextureManager.hpp"

SDL_Renderer* Game::renderer = nullptr;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

struct GameObject
{
    int x, y;
    int speed;
    SDL_Texture *texture;
};
Game::Game(){}

Game::~Game(){}

bool Game::init(){
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        isRunning = false;
    }

    window = SDL_CreateWindow("Avoid the Obj", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        isRunning = false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        isRunning = false;
    }

    if (!(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG))
    {
        cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
        isRunning = false;
    }

    return isRunning;
}

void Game::close(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    Mix_CloseAudio();
    SDL_Quit();
}

void Game::run(){
    SDL_Texture *backgroundTexture = TextureManager::Loadtexture("img/BGR.jpg");
    if (!backgroundTexture)
    {
        isRunning = false;
    }

    SDL_Texture *playerTexture = TextureManager::Loadtexture("img/TENLUA.png");
    SDL_Texture *ObjTexture = TextureManager::Loadtexture("img/CNV.png");
    SDL_Texture *HeartTexture = TextureManager::Loadtexture("img/3.png");
    if (!playerTexture || !ObjTexture)
    {
        isRunning =false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_Log("SDL_mixer could not initialize! SDL_mixer Error: %s", Mix_GetError());
        isRunning = false;
    }

    Mix_Music *backgroundMusic = Mix_LoadMUS("BackgroundSound.mp3");
    if (backgroundMusic == nullptr)
    {
        SDL_Log("Failed to load background music! SDL_mixer Error: %s", Mix_GetError());
        isRunning = false;
    }

    Mix_Chunk *explosion = Mix_LoadWAV("explosion.wav");
    if (explosion == nullptr)
    {
        SDL_Log("Failed to load sound effect! SDL_mixer Error: %s", Mix_GetError());
        isRunning = false;
    }

    if (Mix_PlayMusic(backgroundMusic, -1) == -1)
    {
        SDL_Log("Unable to play background music! SDL_mixer Error: %s", Mix_GetError());
        isRunning = false;
    }

    const int FPS = 144;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    GameObject player = {SCREEN_WIDTH / 2, SCREEN_HEIGHT - 50, 5, playerTexture};

    vector<GameObject> Obj;

    bool quit = false;
    SDL_Event event;

    srand(time(0));

    int mang = 3;

    while (!quit)
    {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_LEFT] && player.x > 0)
        {
            player.x -= 5;
        }
        if (keys[SDL_SCANCODE_RIGHT] && player.x < 750)
        {
            player.x += 5;
        }

        if (rand() % 100 < 10)
        {
            GameObject obstacle = {rand() % (SCREEN_WIDTH - 20), 0, 3, ObjTexture};
            Obj.push_back(obstacle);
        }

        for (auto &obstacle : Obj)
        {
            obstacle.y += obstacle.speed;

            if (obstacle.y > SCREEN_HEIGHT)
            {
                Obj.erase(Obj.begin());
            }

            if (player.x < obstacle.x + 20 && player.x + 20 > obstacle.x &&
                player.y < obstacle.y + 20 && player.y + 20 > obstacle.y)
            {
                Obj.clear();
                mang--;
                if (Mix_PlayChannel(1, explosion, 0) == -1)
                {
                    SDL_Log("Unable to play sound effect! SDL_mixer Error: %s", Mix_GetError());
                    isRunning = false;
                }
                Mix_VolumeMusic(MIX_MAX_VOLUME / 2);

                while (Mix_Playing(-1) != 0);

                switch (mang)
                {
                case 1:
                    HeartTexture = TextureManager::Loadtexture("img/1.png");
                    break;
                case 2:
                    HeartTexture = TextureManager::Loadtexture("img/2.png");
                    break;
                case 0:
                    HeartTexture = TextureManager::Loadtexture("img/0.png");
                    break;
                default:
                    break;
                }
                SDL_Delay(1000);
            }
            if (mang <= 0)
            {
                quit = true;
            }
        }

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);

        SDL_Rect playerR = {player.x, player.y, 50, 50};
        SDL_RenderCopy(renderer, player.texture, nullptr, &playerR);

        for (const auto &obstacle : Obj)
        {  
            SDL_Rect objR = {obstacle.x, obstacle.y, 35, 35};
            SDL_RenderCopy(renderer, obstacle.texture, nullptr, &objR);
        }

        SDL_Rect HeartR = {0, 0, 150, 50};
        SDL_RenderCopy(renderer, HeartTexture, NULL, &HeartR);

        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    Mix_FreeChunk(explosion);
    Mix_FreeMusic(backgroundMusic);

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(ObjTexture);
}