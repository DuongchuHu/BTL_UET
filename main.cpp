#include <bits/stdc++.h>
#include <SDL2/SDL_image.h>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

struct GameObject
{
    int x, y;
    int speed;
    SDL_Texture *texture;
};

bool init(SDL_Window *&window, SDL_Renderer *&renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow("Avoid the Obj", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG))
    {
        cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
        return false;
    }

    return true;
}

SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *filePath)
{
    SDL_Surface *surface = IMG_Load(filePath);
    if (!surface)
    {
        cerr << "Failed to load image " << filePath << "! SDL_Error: " << SDL_GetError() << endl;
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
        cerr << "Unable to create texture from " << filePath << "! SDL_Error: " << SDL_GetError() << endl;
        return nullptr;
    }

    return texture;
}

int main(int argc, char *argv[])
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    if (!init(window, renderer))
    {
        return -1;
    }

    SDL_Texture *backgroundTexture = loadTexture(renderer, "img/BGR.jpg");
    if (!backgroundTexture)
    {
        return -1;
    }

    SDL_Texture *playerTexture = loadTexture(renderer, "img/TENLUA.png");
    SDL_Texture *ObjTexture = loadTexture(renderer, "img/CNV.png");
    if (!playerTexture || !ObjTexture)
    {
        return -1;
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
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);

        SDL_Rect playerR = {player.x, player.y, 50, 50};
        SDL_RenderCopy(renderer, player.texture, nullptr, &playerR);

        for (const auto &obstacle : Obj)
        {
            SDL_Rect objR = {obstacle.x, obstacle.y, 35, 35};
            SDL_RenderCopy(renderer, obstacle.texture, nullptr, &objR);
        }

        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if(frameDelay > frameTime){
            SDL_Delay(frameDelay - frameTime);
        }
    }

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(ObjTexture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();

    return 0;
}
