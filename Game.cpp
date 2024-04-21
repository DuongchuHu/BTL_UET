#include "Game.h"
#include "TextureManager.hpp"
// #include "Bullet.h"

SDL_Renderer *Game::renderer = nullptr;
TTF_Font *Game::GameOverFont = nullptr;
// Bullet *bullet = nullptr;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int NewHighScore;
int TotalCoin = 0;

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

    // SDL_Texture* bullet = TextureManager::Loadtexture("Input/img/Bullet.png");

    SDL_Texture *backgroundTexture = TextureManager::Loadtexture("Input/img/BGR.jpg");
    SDL_Texture *playerTexture = TextureManager::Loadtexture("Input/img/TENLUA.png");
    SDL_Texture *ObjTexture = TextureManager::Loadtexture("Input/img/CNV.png");
    SDL_Texture *HeartTexture = TextureManager::Loadtexture("Input/img/3.png");
    SDL_Texture *CoinTexture = TextureManager::Loadtexture("Input/img/coin.png");
    if (!backgroundTexture)
    {
        isRunning = false;
    }
    if (!playerTexture || !ObjTexture || !HeartTexture || !CoinTexture)
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

    bool quit = false;
    SDL_Event event;

    srand(time(0));

    int mang = 3;

    while (!quit)
    {
        frameStart = SDL_GetTicks();
        auto Score = SDL_GetTicks() / 10 - CountTime + 100 * TotalCoin;
        int len = log10(Score + 1) + 1;
        string timestring = "Score: " + to_string(Score);

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

        for (auto it = Obj.begin(); it != Obj.end(); ++it)
        {
            it->y += it->speed;

            if (it->y > SCREEN_HEIGHT)
            {
                Obj.erase(it);
                --it;
                continue;
            }
            // if(it->x + 35 > bullet->xpos && it->x + 35 < bullet->xpos + 25 && it->y < bullet->ypos && it->y + 35 > bullet->ypos){
            //     Obj.erase(it);
            //     --it;
            //     continue;
            // }
            if (player.x < it->x + 20 && player.x + 20 > it->x && player.y < it->y + 20 && player.y + 20 > it->y)
            {
                Obj.clear();
                Coin.clear();
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
                }
                break;
            }
        }

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);

        SDL_Rect playerR = {player.x, player.y, 50, 50};
        SDL_RenderCopy(renderer, player.texture, nullptr, &playerR);
        // if (keys[SDL_SCANCODE_SPACE])
        // {
        //     int time = SDL_GetTicks();
        //     int x = player.x;
        //     int y = player.y;
        //     SDL_Rect BulletRect = {x + 12, y - 50, 25, 50};
        //     SDL_RenderCopy(Game::renderer, bullet->BulletTexture, NULL, &BulletRect);
        //     // bullet->update();
        // }

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
    }

    SDL_Texture *GameOver = TextureManager::LoadFontTexture("Game Over", 100, "Input/ttf/LIT-Mango.ttf", "white");
    SDL_Rect GameOverRect = {150, 100, 478, 125};
    SDL_RenderCopy(renderer, GameOver, nullptr, &GameOverRect);
    SDL_RenderPresent(renderer);

    SDL_Delay(50);

    Mix_FreeChunk(explosion);
    Mix_FreeMusic(backgroundMusic);
    Mix_FreeChunk(CoinLoud);

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(ObjTexture);
    SDL_DestroyTexture(GameOver);
    SDL_DestroyTexture(CoinTexture);
    SDL_DestroyTexture(HeartTexture);
    pushScore();

    isRunning = false;
}
void Game::printMainScreen()
{
    SDL_Texture *MainScreenTexture = TextureManager::Loadtexture("Input/img/MainScreen1.jpg");
    SDL_RenderCopy(renderer, MainScreenTexture, NULL, NULL);

    SDL_Texture *NameGame = TextureManager::LoadFontTexture("SPACESHIP", 100, "Input/ttf/MTO CHANEY.TTF", "white");
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
                    SDL_DestroyTexture(MainScreenTexture);
                    SDL_DestroyTexture(PlayText);
                    SDL_DestroyTexture(ScoreText);
                    SDL_DestroyTexture(NameGame);
                    SwitchScreen(2);
                }
                else if (x >= 250 && x <= 250 + 300 && y >= 325 && y <= 325 + 125)
                {
                    quit = true;
                    Mix_FreeMusic(MainScreenMusic);
                    SDL_DestroyTexture(MainScreenTexture);
                    SDL_DestroyTexture(PlayText);
                    SDL_DestroyTexture(ScoreText);
                    SDL_DestroyTexture(NameGame);
                    SwitchScreen(3);
                }
                else if (x >= 275 && x <= 275 + 250 && y >= 450 && y <= 450 + 100)
                {
                    quit = true;
                    Mix_FreeMusic(MainScreenMusic);
                    SDL_DestroyTexture(MainScreenTexture);
                    SDL_DestroyTexture(PlayText);
                    SDL_DestroyTexture(ScoreText);
                    SDL_DestroyTexture(NameGame);
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

    int OldHighScore;
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

    SDL_Texture *HighScoreText = TextureManager::LoadFontTexture("High Score:", 100, "Input/ttf/MTO Telephone.ttf", "black");
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
    SDL_Texture *ScoreText = TextureManager::LoadFontTexture(score.c_str(), 100, "Input/ttf/MTO Telephone.ttf", "black");
    SDL_Rect ScoreRect = {395 - ScoreW / 2, 255, ScoreW, 75};
    SDL_RenderCopy(renderer, ScoreText, NULL, &ScoreRect);
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
                    SDL_DestroyTexture(ScoreTexture);
                    SDL_DestroyTexture(ESCTexture);
                    SDL_DestroyTexture(ScoreText);
                    SDL_DestroyTexture(HighScoreText);
                    quit = true;
                    SwitchScreen(1);
                }
            }
        }
    }
}
void Game::printGameModeScreen()
{
    SDL_Texture *Screen = TextureManager::Loadtexture("Input/img/Score.png");
    SDL_RenderCopy(renderer, Screen, NULL, NULL);

    SDL_Texture *ESCTexture = TextureManager::Loadtexture("Input/img/ESC.png");
    SDL_Rect ESCRect = {755, 0, 35, 35};
    SDL_RenderCopy(renderer, ESCTexture, NULL, &ESCRect);


    // SDL_Texture *GameMode = TextureManager::LoadFontTexture("Game Mode", 100, "Input/ttf/MTO Astro City.ttf", "white");
    // SDL_Texture *EasyMode = TextureManager::LoadFontTexture("Easy", 100, "Input/ttf/MTO Astro City.ttf", "white");
    // SDL_Texture *MediumMode = TextureManager::LoadFontTexture("Medium", 100, "Input/ttf/MTO Astro City.ttf", "white");
    // SDL_Texture *HardMode = TextureManager::LoadFontTexture("Hard", 100, "Input/ttf/MTO Astro City.ttf", "white");
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

    SDL_RenderPresent(renderer);
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
