#include "../include/Sprite.h"
#include "../include/ResourceManager.h"
#include <iostream>

//   use loaded `shared_ptr<SDL_Texture>`
Sprite::Sprite(SDL_Renderer* renderer, std::shared_ptr<SDL_Texture> texture, float x, float y, float speed)
    : mRenderer(renderer), mTexture(texture), mSpeed(speed), xPositiveDirection(true) {

    mSprite.x = x;
    mSprite.y = y;
    mSprite.w = 40.0f;
    mSprite.h = 40.0f;
}

//  load BMP
Sprite::Sprite(SDL_Renderer* renderer, const std::string& filePath, float x, float y, float speed)
    : mRenderer(renderer), mSpeed(speed), xPositiveDirection(true) {

    mSprite.x = x;
    mSprite.y = y;
    mSprite.w = 40.0f;
    mSprite.h = 40.0f;

    //  ResourceManager load textture
    mTexture = ResourceManager::GetInstance().LoadTexture(renderer, filePath);

    if (!mTexture) {
        std::cerr << "[ERROR] Failed to load texture: " << filePath << std::endl;
    }
}

//  `shared_ptr` 
Sprite::~Sprite() {}

void Sprite::Update(float deltaTime) {
    float movement = mSpeed * deltaTime;

    // Spaceship move
    if (mSprite.y >= 500) {  // 让 Spaceship 只在底部
        if (xPositiveDirection) {
            mSprite.x += movement;
        } else {
            mSprite.x -= movement;
        }
    }

    //   Spaceship bouncing
    if (mSprite.x <= 0 || mSprite.x + mSprite.w >= 800) {
        xPositiveDirection = !xPositiveDirection;
        mSprite.x = std::max(0.0f, std::min(mSprite.x, 800.0f - mSprite.w));
    }
}

void Sprite::Render() {
    if (mTexture) {
        SDL_RenderCopyF(mRenderer, mTexture.get(), nullptr, &mSprite);
    }
}
