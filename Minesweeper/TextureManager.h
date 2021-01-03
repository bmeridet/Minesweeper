#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

#pragma once

class TextureManager
{
public:
	static sf::Texture& GetTexture(const char* textureKey);
	static void Clear();

private:
	static std::unordered_map<std::string, sf::Texture> textures;
	static void LoadTexture(const char* fileName);
};

