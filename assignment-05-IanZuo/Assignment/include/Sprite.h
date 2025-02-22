#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>
#include <memory>
#include <string>

class Sprite {
public:
    Sprite(SDL_Renderer* renderer, std::shared_ptr<SDL_Texture> texture, float x, float y, float speed);

    Sprite(SDL_Renderer* renderer, const std::string& filePath, float x, float y, float speed);

    ~Sprite();

    void Update(float deltaTime);
    void Render();

    float GetX() const { return mSprite.x; }
    float GetY() const { return mSprite.y; }
    void SetX(float x) { mSprite.x = x; }
    void SetY(float y) { mSprite.y = y; }

private:
    SDL_FRect mSprite;
    float mSpeed;
    bool xPositiveDirection;
    bool yPositiveDirection;
    std::shared_ptr<SDL_Texture> mTexture; 
    SDL_Renderer* mRenderer;
};

#endif // SPRITE_H
