#pragma once
#include "Animation.h"

enum class AnimationName {
	CAT,
	CAT_HAPPY
};

class AnimationManager
{
public:
	AnimationManager(){}
	void update(float dt);
	void render();
	void addAnimation(AnimationName name, Animation animation);
	void attachAnimation(std::shared_ptr<IAnimated> object, AnimationName name);
	Animation& getAnimation(AnimationName name);
	Animation& getAnimation(std::shared_ptr<IAnimated> object);

private:
	std::unordered_map<AnimationName, Animation> mAnimationMap;
	std::unordered_map<std::shared_ptr<IAnimated>, Animation> mAnimations;
};
