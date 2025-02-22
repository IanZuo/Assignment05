#ifndef APPLICATION_H
#define APPLICATION_H

#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include "Sprite.h"

class Application {
public:
    Application();
    ~Application();
    
    bool Initialize();
    void Run(int targetFPS);
    void Shutdown();

private:
    void ProcessInput();
    void Update(float deltaTime, float& enemyMoveDirection);
    void Render();

    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    bool mRun;

    // Enemy grid and movement tracking
    std::vector<std::unique_ptr<Sprite>> mEnemies;
    int enemyDirection; // 1 = Right, -1 = Left
    int movedDistance;  // Distance moved before switching direction
    const int moveThreshold = 100; // Pixels before enemies change direction
    const int enemySpeed = 2;  // Speed of enemy movement

    // Player spaceship
    std::unique_ptr<Sprite> mainCharacter;
    const int spaceshipSpeed = 5; // Speed of player movement
};

#endif

