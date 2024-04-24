#include "Game.h"
#include "TextureManager.hpp"

SDL_Renderer *Game::renderer = nullptr;
TTF_Font *Game::GameOverFont = nullptr;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int NewHighScore;

struct GameObject
{
    int x, y;
    int speed;
    SDL_Texture *texture;
};
Game::Game() {}

Game::~Game() {}

bool Game::init()
{
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
    if (TTF_Init() == -1)
    {
        cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
        isRunning = false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_Log("SDL_mixer could not initialize! SDL_mixer Error: %s", Mix_GetError());
        isRunning = false;
    }

    return isRunning;
}

void Game::close()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(GameOverFont);

    IMG_Quit();
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}

void Game::printPlayScreen()
{
    CountTime = SDL_GetTicks() / 10;
    int TotalCoin = 0;

    SDL_Texture *BulletTexture = TextureManager::Loadtexture("Input/img/Bullet.png");
    SDL_Texture *backgroundTexture = TextureManager::Loadtexture("Input/img/BGR.jpg");
    SDL_Texture *playerTexture = TextureManager::Loadtexture("Input/img/SuperPacman.png");
    SDL_Texture *ObjTexture = TextureManager::Loadtexture("Input/img/CNV.png");
    SDL_Texture *HeartTexture = TextureManager::Loadtexture("Input/img/3.png");
    SDL_Texture *CoinTexture = TextureManager::Loadtexture("Input/img/coin.png");
    if (!backgroundTexture)
    {
        isRunning = false;
    }
    if (!playerTexture || !ObjTexture || !HeartTexture || !CoinTexture || !BulletTexture)
    {
        isRunning = false;
    }

    Mix_Music *backgroundMusic = Mix_LoadMUS("Input/mixer/BackgroundSound.mp3");
    if (backgroundMusic == nullptr)
    {
        SDL_Log("Failed to load background music! SDL_mixer Error: %s", Mix_GetError());
        isRunning = false;
    }

    Mix_Chunk *explosion = Mix_LoadWAV("Input/mixer/explosion.wav");
    Mix_Chunk *CoinLoud = Mix_LoadWAV("Input/mixer/Coin.mp3");
    if (explosion == nullptr || CoinLoud == nullptr)
    {
        SDL_Log("Failed to load sound effect! SDL_mixer Error: %s", Mix_GetError());
        isRunning = false;
    }

    if (Mix_PlayMusic(backgroundMusic, -1) == -1)
    {
        SDL_Log("Unable to play background music! SDL_mixer Error: %s", Mix_GetError());
        isRunning = false;
    }

    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    GameObject player = {SCREEN_WIDTH / 2, SCREEN_HEIGHT - 50, 5, playerTexture};
    vector<GameObject> Obj;
    vector<GameObject> Coin;
    vector<GameObject> BulletVector;

    bool quit = false;
    SDL_Event event;

    srand(time(0));

    int mang = 3;
    int ShootTime = 0;

    while (!quit)
    {   
        bool lose = false;
        frameStart = SDL_GetTicks();
        auto Score = SDL_GetTicks() / 10 - CountTime + 100 * TotalCoin;
        int len = log10(Score + 1) + 1;
        string timestring = "Score: " + to_string(Score);

        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
                isPlaying = false;
                // isRunning = false;
            }
        }
        if (isPlaying == true)
        {
            const Uint8 *keys = SDL_GetKeyboardState(NULL);
            if (keys[SDL_SCANCODE_LEFT] && player.x > 0)
            {
                player.x -= 5;
            }
            if (keys[SDL_SCANCODE_RIGHT] && player.x < 750)
            {
                player.x += 5;
            }
            if (keys[SDL_SCANCODE_UP] && player.y > 0)
            {
                player.y -= 5;
            }
            if (keys[SDL_SCANCODE_DOWN] && player.y < 550)
            {
                player.y += 5;
            }

            if (rand() % 100 < number)
            {
                GameObject obstacle = {rand() % (SCREEN_WIDTH - 20), 0, 3, ObjTexture};
                Obj.push_back(obstacle);
            }
            if (rand() % 100 < 2)
            {
                GameObject CoinObj = {rand() % (SCREEN_WIDTH - 20), 0, 5, CoinTexture};
                Coin.push_back(CoinObj);
            }

            bool soundPlaying = false;

            for (auto it = Coin.begin(); it != Coin.end(); ++it)
            {
                it->y += it->speed;

                if (it->y > SCREEN_HEIGHT)
                {
                    Coin.erase(it);
                    it--;
                    continue;
                }
                if (player.x < it->x + 25 && player.x + 25 > it->x && player.y < it->y + 25 && player.y + 25 > it->y)
                {
                    TotalCoin += 1;
                    Coin.erase(it);
                    it--;

                    if (!soundPlaying && Mix_PlayChannel(1, CoinLoud, 0) != -1)
                    {
                        soundPlaying = true;
                    }
                }
            }
            if (soundPlaying && Mix_Playing(1) == 0)
            {
                soundPlaying = false;
            }

            if (SDL_GetTicks() - ShootTime >= 1000)
            {
                GameObject bullet = {player.x + 10, player.y, 5, BulletTexture};
                BulletVector.push_back(bullet);
                ShootTime = SDL_GetTicks();
            }

            for (auto it = BulletVector.begin(); it != BulletVector.end(); it++)
            {
                it->y -= it->speed;
                if (it->y < 0)
                {
                    BulletVector.erase(it);
                    --it;
                    continue;
                }
            }

            for (auto it = Obj.begin(); it != Obj.end(); ++it)
            {
                it->y += it->speed;

                if (it->y > SCREEN_HEIGHT)
                {
                    Obj.erase(it);
                    --it;
                    continue;
                }
                if (player.x < it->x + 20 && player.x + 20 > it->x && player.y < it->y + 20 && player.y + 20 > it->y)
                {
                    Obj.clear();
                    Coin.clear();
                    BulletVector.clear();
                    --mang;
                    if (Mix_PlayChannel(1, explosion, 0) == -1)
                    {
                        SDL_Log("Unable to play sound effect! SDL_mixer Error: %s", Mix_GetError());
                        isRunning = false;
                    }
                    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);

                    while (Mix_Playing(-1) != 0)
                        ;

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
                    SDL_Delay(200);
                    if (mang <= 0)
                    {
                        quit = true;
                        player.texture = nullptr;
                        lose = true;
                    }
                    break;
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
            for (const auto &CoinObj : Coin)
            {
                SDL_Rect CoinR = {CoinObj.x, CoinObj.y, 35, 35};
                SDL_RenderCopy(renderer, CoinObj.texture, nullptr, &CoinR);
            }
            for (const auto &bullet : BulletVector)
            {
                SDL_Rect BulletR = {bullet.x, bullet.y, 35, 35};
                SDL_RenderCopy(renderer, bullet.texture, nullptr, &BulletR);
            }
            SDL_Rect HeartR = {0, 0, 150, 50};
            SDL_RenderCopy(renderer, HeartTexture, NULL, &HeartR);

            SDL_Texture *TimeTexture = TextureManager::LoadFontTexture(timestring.c_str(), 50, "Input/ttf/MTO Astro City.ttf", "yellow");
            SDL_Rect TimeRect = {SCREEN_WIDTH - 100 - 10 * len, 0, 70 + 10 * len, 50};
            SDL_RenderCopy(renderer, TimeTexture, NULL, &TimeRect);
            SDL_DestroyTexture(TimeTexture);

            SDL_RenderPresent(renderer);

            frameTime = SDL_GetTicks() - frameStart;
            if (frameDelay > frameTime)
            {
                SDL_Delay(frameDelay - frameTime);
            }
            NewHighScore = Score;
            if(lose == true){
            printGameOverScreen();}
        }
    }

    SDL_RenderPresent(renderer);

    SDL_Delay(200);

    Mix_FreeChunk(explosion);
    Mix_FreeMusic(backgroundMusic);
    Mix_FreeChunk(CoinLoud);

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(ObjTexture);
    SDL_DestroyTexture(CoinTexture);
    SDL_DestroyTexture(HeartTexture);
    SDL_DestroyTexture(BulletTexture);
    pushScore();
    isRunning = false;
}
void Game::printMainScreen()
{
    SDL_Texture *MainScreenTexture = TextureManager::Loadtexture("Input/img/MainScreen1.jpg");
    SDL_RenderCopy(renderer, MainScreenTexture, NULL, NULL);

    SDL_Texture *NameGame = TextureManager::LoadFontTexture("SUPERPACMAN", 100, "Input/ttf/MTO CHANEY.TTF", "white");
    SDL_Rect NameGameRect = {150, 100, 478, 125};
    SDL_RenderCopy(renderer, NameGame, NULL, &NameGameRect);

    SDL_Texture *PlayText = TextureManager::LoadFontTexture("PLAY", 50, "Input/ttf/MTO CHANEY.TTF", "white");
    SDL_Rect PlayTextRect = {285, 225, 225, 100};
    SDL_RenderCopy(renderer, PlayText, NULL, &PlayTextRect);

    SDL_Texture *ScoreText = TextureManager::LoadFontTexture("HIGH SCORE", 50, "Input/ttf/MTO CHANEY.TTF", "white");
    SDL_Rect ScoreTextRect = {250, 325, 300, 125};
    SDL_RenderCopy(renderer, ScoreText, NULL, &ScoreTextRect);

    SDL_Texture *GameModeText = TextureManager::LoadFontTexture("GAME MODE", 50, "Input/ttf/MTO CHANEY.TTF", "white");
    SDL_Rect GameModeRect = {275, 450, 250, 100};
    SDL_RenderCopy(renderer, GameModeText, NULL, &GameModeRect);

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
    SDL_DestroyTexture(MainScreenTexture);
    SDL_DestroyTexture(PlayText);
    SDL_DestroyTexture(ScoreText);
    SDL_DestroyTexture(NameGame);
    SDL_DestroyTexture(GameModeText);
    bool quit = false;
    SDL_Event event;

    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
                isRunning = false;
                break;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN && isMainScreen == true)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (x >= 285 && x <= 285 + 225 && y >= 225 && y <= 225 + 100)
                {
                    quit = true;
                    Mix_FreeMusic(MainScreenMusic);
                    SwitchScreen(2);
                }
                else if (x >= 250 && x <= 250 + 300 && y >= 325 && y <= 325 + 125)
                {
                    quit = true;
                    Mix_FreeMusic(MainScreenMusic);
                    SwitchScreen(3);
                }
                else if (x >= 275 && x <= 275 + 250 && y >= 450 && y <= 450 + 100)
                {
                    quit = true;
                    Mix_FreeMusic(MainScreenMusic);
                    SwitchScreen(4);
                }
            }
        }
    }
}
void Game::pushScore()
{
    ifstream input_file("HighScore.txt");
    ofstream output_file("HighScore.txt.tmp");

    if (!input_file.is_open() || !output_file.is_open())
    {
        cerr << "Không thể mở tệp đầu ra." << endl;
        isRunning = false;
    }

    long long OldHighScore;
    input_file >> OldHighScore;

    if (NewHighScore > OldHighScore)
    {
        output_file << NewHighScore;
    }
    else
    {
        output_file << OldHighScore;
    }

    input_file.close();
    output_file.close();

    remove("HighScore.txt");
    rename("HighScore.txt.tmp", "HighScore.txt");
}
void Game::printScoreScreen()
{
    SDL_Texture *ScoreTexture = TextureManager::Loadtexture("Input/img/Score.png");
    SDL_RenderCopy(renderer, ScoreTexture, NULL, NULL);

    SDL_Texture *HighScoreText = TextureManager::LoadFontTexture("High Score:", 100, "Input/ttf/LIT-Mango.ttf", "black");
    SDL_Rect HighScoreRect = {250, 175, 300, 100};
    SDL_RenderCopy(renderer, HighScoreText, NULL, &HighScoreRect);

    SDL_Texture *ESCTexture = TextureManager::Loadtexture("Input/img/ESC.png");
    SDL_Rect ESCRect = {755, 0, 35, 35};
    SDL_RenderCopy(renderer, ESCTexture, NULL, &ESCRect);

    ifstream in;
    in.open("HighScore.txt");

    int HighScore;
    in >> HighScore;
    int len = log10(HighScore + 1) + 1;
    int ScoreW = 25 * len;

    string score = to_string(HighScore);
    SDL_Texture *ScoreText = TextureManager::LoadFontTexture(score.c_str(), 100, "Input/ttf/LIT-Mango.ttf", "black");
    SDL_Rect ScoreRect = {395 - ScoreW / 2, 255, ScoreW, 75};
    SDL_RenderCopy(renderer, ScoreText, NULL, &ScoreRect);

    SDL_DestroyTexture(ScoreTexture);
    SDL_DestroyTexture(ESCTexture);
    SDL_DestroyTexture(ScoreText);
    SDL_DestroyTexture(HighScoreText);

    SDL_RenderPresent(renderer);

    bool quit = false;
    SDL_Event event;

    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
                isRunning = false;
                break;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN && isMainScreen == true)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (x >= 755 && x <= 755 + 35 && y >= 0 && y <= 35)
                {
                    quit = true;
                    SwitchScreen(1);
                }
            }
        }
    }
}
void Game::printGameModeScreen()
{
    SDL_Texture *Screen = TextureManager::Loadtexture("Input/img/GameMode.jpg");
    SDL_RenderCopy(renderer, Screen, NULL, NULL);
    SDL_DestroyTexture(Screen);

    SDL_Texture *ESCTexture = TextureManager::Loadtexture("Input/img/ESC.png");
    SDL_Rect ESCRect = {755, 0, 35, 35};
    SDL_RenderCopy(renderer, ESCTexture, NULL, &ESCRect);
    SDL_DestroyTexture(ESCTexture);

    SDL_Texture *GameMode = TextureManager::LoadFontTexture("Game Mode", 100, "Input/ttf/MTO Astro City.ttf", "white");
    SDL_Rect GameModeRect = {150, 100, 478, 125};
    SDL_RenderCopy(renderer, GameMode, NULL, &GameModeRect);
    SDL_DestroyTexture(GameMode);

    SDL_Texture *EasyMode = TextureManager::LoadFontTexture("Easy", 100, "Input/ttf/MTO Astro City.ttf", "red");
    SDL_Rect EasyRect = {285, 225, 225, 100};
    SDL_RenderCopy(renderer, EasyMode, NULL, &EasyRect);
    SDL_DestroyTexture(EasyMode);

    SDL_Texture *MediumMode = TextureManager::LoadFontTexture("Medium", 100, "Input/ttf/MTO Astro City.ttf", "yellow");
    SDL_Rect MediumRect = {250, 325, 300, 125};
    SDL_RenderCopy(renderer, MediumMode, NULL, &MediumRect);
    SDL_DestroyTexture(MediumMode);

    SDL_Texture *HardMode = TextureManager::LoadFontTexture("Hard", 100, "Input/ttf/MTO Astro City.ttf", "blue");
    SDL_Rect HardRect = {275, 450, 250, 100};
    SDL_RenderCopy(renderer, HardMode, NULL, &HardRect);
    SDL_DestroyTexture(HardMode);

    SDL_RenderPresent(renderer);

    bool quit = false;
    SDL_Event event;
    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
                isRunning = false;
                break;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN && isMainScreen == true)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (x >= 755 && x <= 755 + 35 && y >= 0 && y <= 35)
                {
                    quit = true;
                    SwitchScreen(1);
                }
                else if (x >= 285 && x <= 285 + 225 && y >= 225 && y <= 225 + 100)
                {
                    quit = true;
                    SwitchMode(1);
                    SwitchScreen(1);
                }
                else if (x >= 250 && x <= 250 + 300 && y >= 325 && y <= 325 + 125)
                {
                    quit = true;
                    SwitchMode(2);
                    SwitchScreen(1);
                }
                else if (x >= 275 && x <= 275 + 250 && y >= 450 && y <= 450 + 100)
                {
                    quit = true;
                    SwitchMode(3);
                    SwitchScreen(1);
                }
            }
        }
    }
}
void Game::run()
{
    if (isMainScreen)
    {
        printMainScreen();
    }
}
void Game::SwitchScreen(int x)
{
    switch (x)
    {
    case 1:
        SDL_RenderClear(renderer);
        printMainScreen();
        isMainScreen = true;
        isPlayScreen = false;
        isScoreScreen = false;
        isGameModeScreen = false;
        break;
    case 2:
        SDL_RenderClear(renderer);
        printPlayScreen();
        isMainScreen = false;
        isPlayScreen = true;
        isScoreScreen = false;
        isGameModeScreen = false;
        break;
    case 3:
        SDL_RenderClear(renderer);
        printScoreScreen();
        isMainScreen = false;
        isPlayScreen = false;
        isScoreScreen = true;
        isGameModeScreen = false;
        break;
    case 4:
        SDL_RenderClear(renderer);
        printGameModeScreen();
        isMainScreen = false;
        isPlayScreen = false;
        isScoreScreen = false;
        isGameModeScreen = true;
        break;
    default:
        break;
    }
}
void Game::printGameOverScreen()
{
    SDL_RenderClear(renderer);
    SDL_Texture *ScreenTexture = TextureManager::Loadtexture("Input/img/GameOverScreen.jpg");
    SDL_RenderCopy(renderer, ScreenTexture, NULL, NULL);
    SDL_DestroyTexture(ScreenTexture);

    SDL_Texture *ExitText = TextureManager::LoadFontTexture("EXIT", 50, "Input/ttf/MTO Telephone.ttf", "yellow");
    SDL_Rect ExitRect = {350, 435, 100, 100};
    SDL_RenderCopy(renderer, ExitText, NULL, &ExitRect);
    SDL_DestroyTexture(ExitText);

    SDL_Texture *PlayAgainText = TextureManager::LoadFontTexture("PLAY AGAIN", 50, "Input/ttf/MTO Telephone.ttf", "yellow");
    SDL_Rect PlayAgainRect = {275, 355, 250, 100};
    SDL_RenderCopy(renderer, PlayAgainText, NULL, &PlayAgainRect);
    SDL_DestroyTexture(PlayAgainText);

    SDL_RenderPresent(renderer);

    bool quit = false;
    SDL_Event event;
    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
                isRunning = false;
                break;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (x >= 350 && x <= 350 + 100 && y >= 435 && y <= 435 + 100)
                {
                    quit = true;
                    isRunning = false;
                }
                if (x >= 275 && x <= 275 + 250 && y >= 355 && y <= 355 + 100)
                {
                    quit = true;
                    isRunning = true;
                    printPlayScreen();
                }
            }
        }
    }
}