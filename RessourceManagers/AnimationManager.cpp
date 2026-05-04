#include "AnimationManager.h"



void AnimationManager::update(float dt)
{
	for (auto& item : mAnimations)
	{
		item.second.update(dt);
		auto rect = item.second.getFrame();
		item.first->setTexRect(rect);
	}
}


void AnimationManager::addAnimation(AnimationName name, Animation animation)
{
	if (mAnimationMap.find(name) == mAnimationMap.end())
	{
		mAnimationMap.insert({ name, animation });
	}
}

void AnimationManager::attachAnimation(std::shared_ptr<IAnimated> object, AnimationName name)
{
	auto it = mAnimationMap.find(name);
	if (it != mAnimationMap.end())
	{
		auto& animation = it->second;
		object->setTexture(animation.getTexture());
		auto jt = mAnimations.find(object);
		if (jt == mAnimations.end())
		{
			mAnimations.insert({ object, animation });
		}
		else
		{
			mAnimations.at(object) = animation;
		}
	}
}

Animation& AnimationManager::getAnimation(AnimationName name)
{
	auto it = mAnimationMap.find(name);
	if (it != mAnimationMap.end())
	{
		return it->second;
	}
}

Animation& AnimationManager::getAnimation(std::shared_ptr<IAnimated> object)
{
	auto it = mAnimations.find(object);
	if (it != mAnimations.end())
	{
		return it->second;
	}
}

