#pragma once
#include "Animation.h"

enum class AnimationName {
	CAT,
	CAT_HAPPY
};

class AnimationManager
{
public:
	AnimationManager(AnimationManager& other) = delete;
	void operator=(const AnimationManager& other) = delete;

	void update(float dt);
	void render();
	static AnimationManager* getInstance();
	void addAnimation(AnimationName name, Animation animation);
	void attachAnimation(std::shared_ptr<IAnimated> object, AnimationName name);
	Animation& getAnimation(AnimationName name);
	Animation& getAnimation(std::shared_ptr<IAnimated> object);

protected:
	AnimationManager(){}
	static AnimationManager* mInstance;
private:
	std::unordered_map<AnimationName, Animation> mAnimationMap;
	std::unordered_map<std::shared_ptr<IAnimated>, Animation> mAnimations;
};
