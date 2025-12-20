#include "FontManager.h"


FontManager* FontManager::mInstance = nullptr;

FontManager* FontManager::getInstance()
{
	if (mInstance == nullptr) mInstance = new FontManager();
	return mInstance;
}

