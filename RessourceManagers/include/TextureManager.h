#pragma once
#include "SFML//Graphics.hpp"
#include <iostream>

enum class Texture {
	SPRITESHEET,
	TITLE_MENU,
	LOADING_SCREEN,
	PARTICLE,
	GAME_BG,
	LITTER_BOX,
	INVENTORY,
	ANIMATION_SHEET
};

class TextureManager
{
public:
	TextureManager(TextureManager& other) = delete;
	void operator=(const TextureManager& other) = delete;

	static TextureManager* getInstance();
	
	void loadTexture(Texture texture, const std::string& filename)
	{
		sf::Texture tex;
		if (!tex.loadFromFile(filename)) std::cout << "Error loading texture from file" << std::endl;;
		mTextures.insert({ texture, std::make_shared<sf::Texture>(tex)});
	}

	std::shared_ptr<sf::Texture> getTexture(Texture texture)
	{
		return mTextures[texture];
	}

protected:
	TextureManager() {};
	static TextureManager* mInstance;

private:
	
	std::unordered_map<Texture, std::shared_ptr<sf::Texture> > mTextures;
};