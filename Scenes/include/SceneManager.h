#pragma once
#include <memory>
#include <stack>

class Scene;

class SceneManager {

public:
	
	SceneManager(SceneManager& other) = delete;
	void operator=(const SceneManager& other) = delete;
	static SceneManager* getInstance();

	void addScene(std::shared_ptr<Scene> scene) { mScenes.push(std::move(scene)); };
	std::shared_ptr<Scene> getScene() { return mScenes.top(); }
	void removeScene() { mScenes.pop(); };

private:
	SceneManager() {};
	static SceneManager* mInstance;
	std::stack<std::shared_ptr<Scene>> mScenes;
};