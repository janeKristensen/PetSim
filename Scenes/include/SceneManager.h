#pragma once
#include <memory>
#include <stack>

class Scene;

class SceneManager {

public:
	
	SceneManager(SceneManager& other) = delete;
	void operator=(const SceneManager& other) = delete;
	static SceneManager* getInstance();

	void replaceScene(std::shared_ptr<Scene> scene);
	void changeScene(std::shared_ptr<Scene> scene);
	std::shared_ptr<Scene> getScene() { return mScenes.top(); }
	void removeScene();

private:
	SceneManager() {};
	static SceneManager* mInstance;
	std::shared_ptr<Scene> mCurrentScene;
	std::stack<std::shared_ptr<Scene>> mScenes;
};