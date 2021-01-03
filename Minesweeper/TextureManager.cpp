#include "TextureManager.h"

std::unordered_map<std::string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(const char* fileName)
{
	std::string filePath = "images/";
	filePath += fileName;
	filePath += ".png";

	textures[fileName].loadFromFile(filePath);
}

sf::Texture& TextureManager::GetTexture(const char* textureKey)
{
	if (textures.find(textureKey) == textures.end())
		LoadTexture(textureKey);

	return textures[textureKey];
}

void TextureManager::Clear()
{
	textures.clear();
}