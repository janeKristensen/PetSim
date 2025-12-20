#include "Texturemanager.h"

TextureManager* TextureManager::mInstance = nullptr;


TextureManager* TextureManager::getInstance()
{
	if (mInstance == nullptr) mInstance = new TextureManager();
	return mInstance;
}

