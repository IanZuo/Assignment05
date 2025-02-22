#include "../include/ResourceManager.h"

// getinstance
ResourceManager& ResourceManager::GetInstance() {
    static ResourceManager instance;
    return instance;
}

// load bmp texture
std::shared_ptr<SDL_Texture> ResourceManager::LoadTexture(SDL_Renderer* renderer, const std::string& filePath) {
    auto it = textures.find(filePath);
    if (it != textures.end()) {
        return it->second;  // return texture loaded
    }

    // test filepath
    std::cout << "[INFO] Loading texture: " << filePath << std::endl;

    // load bmp
    SDL_Surface* surface = SDL_LoadBMP(filePath.c_str());
    if (!surface) {
        std::cerr << "[ERROR] Failed to load BMP: " << filePath << " | SDL_Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    // create texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "[ERROR] Failed to create texture from BMP: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    // save to hashtable
    auto sharedTexture = std::shared_ptr<SDL_Texture>(texture, SDL_DestroyTexture);
    textures[filePath] = sharedTexture;
    return sharedTexture;
}

// get loaded texture
std::shared_ptr<SDL_Texture> ResourceManager::GetTexture(const std::string& filePath) {
    auto it = textures.find(filePath);
    if (it != textures.end()) {
        return it->second;
    }
    return nullptr;
}

// release resources
void ResourceManager::Shutdown() {
    textures.clear();
    std::cout << "[INFO] ResourceManager: All textures have been freed." << std::endl;
}
