#include "SceneManager.h"

SceneManager* SceneManager::mInstance = nullptr;

SceneManager* SceneManager::getInstance()
{
	if (mInstance == nullptr) mInstance = new SceneManager();
	return mInstance;
}