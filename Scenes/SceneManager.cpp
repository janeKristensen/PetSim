#include "SceneManager.h"

SceneManager* SceneManager::mInstance = nullptr;

SceneManager* SceneManager::getInstance()
{
	if (mInstance == nullptr) mInstance = new SceneManager();
	return mInstance;
}


void SceneManager::replaceScene(std::shared_ptr<Scene> scene) 
{ 
	mPreviousScene = mScenes.top();
	mScenes.pop();
	mScenes.push(scene); 
};

void SceneManager::changeScene(std::shared_ptr<Scene> scene)
{
	if (mScenes.size() > 0)
	{
		mPreviousScene = mScenes.top();
	}
	
	mScenes.push(scene);
};

void SceneManager::removeScene() 
{ 
	mPreviousScene = mScenes.top();
	mScenes.pop(); 
};