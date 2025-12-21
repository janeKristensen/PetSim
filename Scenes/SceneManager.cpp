#include "SceneManager.h"

SceneManager* SceneManager::mInstance = nullptr;

SceneManager* SceneManager::getInstance()
{
	if (mInstance == nullptr) mInstance = new SceneManager();
	return mInstance;
}


void SceneManager::replaceScene(std::shared_ptr<Scene> scene) 
{ 
	mCurrentScene = mScenes.top();
	mScenes.pop();
	mScenes.push(scene); 
};

void SceneManager::changeScene(std::shared_ptr<Scene> scene)
{
	mScenes.push(scene);
};

void SceneManager::removeScene() 
{ 
	mCurrentScene = mScenes.top();
	mScenes.pop(); 
};