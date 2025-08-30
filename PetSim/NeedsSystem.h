#pragma once
#include <memory>
#include <future>
#include "Pet.h"
#include "Model.h"
#include "Item.h"


enum class State {LOW, MEDIUM, HIGH, COUNT};

class NeedsSystem {

public:
	NeedsSystem(std::shared_ptr<Pet> pet, std::shared_future<std::shared_ptr<Model>> model) : mPet(pet), mModelFuture(model){}
	void update(float dt);
	void processItem(Item& item);

private:
	void decayValues(float dt);
	void updateSystemPrompt();
	void updateState();
	void addToFeed(int32_t value) { mPet->setHungerValue(value); }
	void addToGroom(int32_t value) { mPet->setGroomValue(value); }

	std::shared_ptr<Pet> mPet = nullptr;
	std::shared_ptr<Model> mModel = nullptr;
	std::shared_future<std::shared_ptr<Model>> mModelFuture;
	float mTimeTracker = 0.f;
	State mFeedState = State::HIGH;
	State mGroomState = State::HIGH;
	std::pair<int32_t, int32_t> mPreviousState;

	const char* mStateTable[(int32_t)State::COUNT][(int32_t)State::COUNT] = {

		// groom = LOW, MEDIUM, HIGH
		{ "Urgently need Feeding and Grooming. Very unhappy.",	"Urgently needs feeding and maybe a groom.",	"Urgently needs Feeding." },   // hunger = LOW
		{ "Urgently needs grooming and maybe a feeding.",       "Fairly content.",								"Very happy but could eat." },      // hunger = MEDIUM
		{ "Urgently needs a grooming but is not hungry.",		"Full belly but needs a little grooming.",		"Very happy and content with everything" }   // hunger = HIGH
	};
};
