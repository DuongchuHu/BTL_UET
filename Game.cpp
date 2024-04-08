#include "Game.h"
#include "TextureManager.hpp"

SDL_Renderer* Game::renderer = nullptr;
TTF_Font* Game::GameOverFont = nullptr;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int NewHightScore;

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

    window = SDL_CreateWindow("BTL_UET", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
    if (TTF_Init() == -1) {
        cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
        isRunning = false;
    }

    return isRunning;
}

void Game::close(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(GameOverFont);

    IMG_Quit();
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}

void Game::printPlayScreen(){
    SDL_Texture *backgroundTexture = TextureManager::Loadtexture("Input/img/BGR.jpg");
    if (!backgroundTexture)
    {
        isRunning = false;
    }

    SDL_Texture *playerTexture = TextureManager::Loadtexture("Input/img/TENLUA.png");
    SDL_Texture *ObjTexture = TextureManager::Loadtexture("Input/img/CNV.png");
    SDL_Texture *HeartTexture = TextureManager::Loadtexture("Input/img/3.png");
    if (!playerTexture || !ObjTexture)
    {
        isRunning =false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_Log("SDL_mixer could not initialize! SDL_mixer Error: %s", Mix_GetError());
        isRunning = false;
    }
    Mix_Music *backgroundMusic = Mix_LoadMUS("Input/mixer/BackgroundSound.mp3");
    if (backgroundMusic == nullptr)
    {
        SDL_Log("Failed to load background music! SDL_mixer Error: %s", Mix_GetError());
        isRunning = false;
    }

    Mix_Chunk *explosion = Mix_LoadWAV("Input/mixer/explosion.wav");
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

            if (player.x < obstacle.x + 20 && player.x + 20 > obstacle.x && player.y < obstacle.y + 20 && player.y + 20 > obstacle.y)
            {
                Obj.clear();
                --mang;
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
                    HeartTexture = TextureManager::Loadtexture("Input/img/1.png");
                    break;
                case 2:
                    HeartTexture = TextureManager::Loadtexture("Input/img/2.png");
                    break;
                case 0:
                    HeartTexture = TextureManager::Loadtexture("Input/img/0.png");
                    break;
                default:
                    break;
                }
                SDL_Delay(1000);
            }
            if (mang <= 0)
            {
                quit = true;
                player.texture = nullptr;
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
        auto Score = SDL_GetTicks() / 10;
        string timestring ="Score: " + to_string(Score);
        SDL_Texture* TimeTexture = TextureManager::LoadFontTexture(timestring.c_str(), 50, "Input/ttf/MTO Astro City.ttf", "yellow");
        SDL_Rect TimeRect = {SCREEN_WIDTH - 200, 0, 100, 50};
        SDL_RenderCopy(renderer, TimeTexture, NULL, &TimeRect);

        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
        NewHightScore = Score;
        pushScore();
    }
    
    SDL_Texture* GameOver = TextureManager::LoadFontTexture("Game Over", 100, "Input/ttf/LIT-Mango.ttf", "white");
    SDL_Rect GameOverRect = {150, 100, 478, 125};
    SDL_RenderCopy(renderer, GameOver, nullptr, &GameOverRect);
    SDL_RenderPresent(renderer);

    SDL_Delay(50);

    Mix_FreeChunk(explosion);
    Mix_FreeMusic(backgroundMusic);

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(ObjTexture);
    SDL_DestroyTexture(GameOver);

    isRunning = false;
}
void Game::printMainScreen(){
    SDL_Texture* MainScreenTexture = TextureManager::Loadtexture("Input/img/MainScreen1.jpg");
    SDL_RenderCopy(renderer, MainScreenTexture, NULL, NULL);

    SDL_Texture* NameGame = TextureManager::LoadFontTexture("SPACESHIP", 100, "Input/ttf/MTO CHANEY.TTF", "white");
    SDL_Rect NameGameRect = {150, 100, 478, 125};
    SDL_RenderCopy(renderer, NameGame, NULL, &NameGameRect);

    SDL_Texture* PlayText = TextureManager::LoadFontTexture("PLAY", 50, "Input/ttf/MTO CHANEY.TTF", "white");
    SDL_Rect PlayTextRect = {285, 225, 225, 100};
    SDL_RenderCopy(renderer, PlayText, NULL, &PlayTextRect);

    SDL_Texture* ScoreText = TextureManager::LoadFontTexture("HIGHT SCORE", 50, "Input/ttf/MTO CHANEY.TTF", "white");
    SDL_Rect ScoreTextRect = {250, 325, 300, 125};
    SDL_RenderCopy(renderer, ScoreText, NULL, &ScoreTextRect);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_Log("SDL_mixer could not initialize! SDL_mixer Error: %s", Mix_GetError());
        isRunning = false;
    }
    Mix_Music *MainScreenMusic = Mix_LoadMUS("Input/mixer/MainScreen.mp3");
    if (MainScreenMusic == nullptr)
    {
        SDL_Log("Failed to load MainScreen Music! SDL_mixer Error: %s", Mix_GetError());
        isRunning = false;
    }

    if (Mix_PlayMusic(MainScreenMusic, -1) == -1)
    {
        SDL_Log("Unable to play MainScreen Music! SDL_mixer Error: %s", Mix_GetError());
        isRunning = false;
    }

    SDL_RenderPresent(renderer);
}
void Game::pushScore(){
    ifstream input_file("HightScore.txt");
    ofstream output_file("HightScore.txt.tmp");

    if (!input_file.is_open() || !output_file.is_open()) {
        cerr << "Không thể mở tệp đầu ra." << endl;
        isRunning = false;
    }

    int OldHightScore;
    input_file >> OldHightScore;

    if (NewHightScore > OldHightScore) {
        output_file << NewHightScore;
    } else {
        output_file << OldHightScore;
    }

    input_file.close();
    output_file.close();

    remove("HightScore.txt");
    rename("HightScore.txt.tmp", "HightScore.txt");
}
void Game::printScoreScreen(){
    SDL_Texture* ScoreTexture = TextureManager::Loadtexture("Input/img/Score.png");
    SDL_RenderCopy(renderer, ScoreTexture, NULL, NULL);

    SDL_Texture* HightScoreText = TextureManager::LoadFontTexture("Hight Score:", 100, "Input/ttf/MTO Telephone.ttf", "black");
    SDL_Rect HightScoreRect= {250, 175, 300, 100};
    SDL_RenderCopy(renderer, HightScoreText, NULL, &HightScoreRect);

    ifstream in;
    in.open("HightScore.txt");

    int HightScore; in >> HightScore;
    int len = log10(HightScore + 1) + 1;
    int ScoreW = 25*len;

    string score = to_string(HightScore);
    SDL_Texture* ScoreText = TextureManager::LoadFontTexture(score.c_str(), 100, "Input/ttf/MTO Telephone.ttf", "black");
    SDL_Rect ScoreRect= {395 - ScoreW / 2, 255, ScoreW, 75};
    SDL_RenderCopy(renderer, ScoreText, NULL, &ScoreRect);
    SDL_RenderPresent(renderer);
}
void Game::run(){
    printMainScreen();

    bool quit = false;
    bool isMainScreen = true;
    SDL_Event event;

    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
                isRunning = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && isMainScreen == true){
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (x >= 285 && x <= 285 + 225 &&
                    y >= 225 && y <= 225 + 100) {
                    printPlayScreen();
                    isMainScreen = false;
                }
                else if (x >= 250 && x <= 250 + 300 &&
                         y >= 325 && y <= 325 + 125) {
                    printScoreScreen();
                    isMainScreen = false;
                }
            }
        }
    }
}