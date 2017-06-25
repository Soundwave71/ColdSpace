//
// Created by Leonardo on 6/26/2017.
//

#ifndef COLDSPACE_TEXTUREMANAGER_H
#define COLDSPACE_TEXTUREMANAGER_H

#include <SFML/Graphics/Texture.hpp>
#include "ResourceManager.h"

class TextureManager: public ResourceManager<TextureManager, sf::Texture>{
public:
    TextureManager(): ResourceManager ("Textures.cfg"){}

    sf::Texture* Load (const std::string& l_path) {
        sf::Texture *texture = new sf::Texture();
        if (!texture->loadFromFile(Utils::GetWorkingDirectory() + l_path)) {
            delete texture;
            texture = nullptr;
            std::cerr << "! Failed to load texture:" << l_path << std::endl;
        }
        return texture;
    }
};
#endif //COLDSPACE_TEXTUREMANAGER_H
