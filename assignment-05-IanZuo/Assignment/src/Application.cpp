#include "../include/Application.h"
#include "../include/ResourceManager.h"
#include <iostream>
#include <algorithm> // for std::min  std::max

Application::Application() : mWindow(nullptr), mRenderer(nullptr), mRun(true) {}

Application::~Application() {
    Shutdown();
}

bool Application::Initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL Init Failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    mWindow = SDL_CreateWindow("Assignment 05", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!mWindow || !mRenderer) {
        std::cerr << "Failed to create window or renderer." << std::endl;
        return false;
    }

    //  load Spaceship and Alien texture
    auto spaceshipTexture = ResourceManager::GetInstance().LoadTexture(mRenderer, "Assignment/Assets/Spaceship.bmp");
    auto alienTexture = ResourceManager::GetInstance().LoadTexture(mRenderer, "Assignment/Assets/Alien.bmp");

    if (!spaceshipTexture || !alienTexture) {
        std::cerr << "[ERROR] Failed to load textures." << std::endl;
        return false;
    }

    //  Spaceship use loaded texture
    mainCharacter = std::make_unique<Sprite>(mRenderer, spaceshipTexture, 380, 550, 400.0f);

    //  3x12 enemy
    float startX = 50.0f, startY = 50.0f;
    float spacingX = 60.0f, spacingY = 50.0f;
    
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 12; ++col) {
            float x = startX + col * spacingX;
            float y = startY + row * spacingY;
            mEnemies.push_back(std::make_unique<Sprite>(mRenderer, alienTexture, x, y, 200.0f));
        }
    }
    return true;
}

void Application::Run(int targetFPS) {
    const int frameDelay = 1000 / targetFPS;
    Uint32 frameStart, frameTime;
    float deltaTime = 0.0f;
    Uint32 lastTime = SDL_GetTicks();
    float enemyMoveDirection = 1.0f;  // 1 to right -1 to left

    while (mRun) {
        frameStart = SDL_GetTicks();
        ProcessInput();

        // deltatime cauculation
        Uint32 currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        Update(deltaTime, enemyMoveDirection);
        Render();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay) SDL_Delay(frameDelay - frameTime);
    }
}

void Application::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            mRun = false;
        }
    }
}

void Application::Update(float deltaTime, float& enemyMoveDirection) {
    static float totalMovement = 0.0f;
    float movementSpeed = 200.0f * deltaTime; // speed control

    // find the edge of enemy
    float leftMost = 800, rightMost = 0;
    for (const auto& enemy : mEnemies) {
        float x = enemy->GetX();
        if (x < leftMost) leftMost = x;
        if (x > rightMost) rightMost = x;
    }

    // bouncing enemy group
    if (leftMost <= 0 || rightMost >= 800 - 40) {  // 800 是窗口宽度，40 是敌人宽度
        enemyMoveDirection *= -1;
    }

    // enemy move
    for (auto& enemy : mEnemies) {
        enemy->SetX(enemy->GetX() + enemyMoveDirection * movementSpeed);
    }

    // update Spaceship
    if (mainCharacter) mainCharacter->Update(deltaTime);
}

void Application::Render() {
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mRenderer);

    if (mainCharacter) mainCharacter->Render();
    for (auto& enemy : mEnemies) {
        enemy->Render();
    }

    SDL_RenderPresent(mRenderer);
}

void Application::Shutdown() {
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}
