#pragma once
#include <memory>
#include <future>
#include "Pet.h"
#include "Model.h"
#include "Item.h"


enum class State {LOW, MEDIUM, HIGH, COUNT};
constexpr float DECAY_TIME = 2.0f;

class NeedsSystem {

public:
	NeedsSystem(std::shared_ptr<Pet> pet) : mPet(pet){}
	void setModel(std::shared_ptr<Model> model);
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
	float mTimeTracker = 0.f;
	State mFeedState;
	State mGroomState;
	std::pair<int32_t, int32_t> mPreviousState;

	const char* mStateTable[(int32_t)State::COUNT][(int32_t)State::COUNT] = {

		// groom = LOW, MEDIUM, HIGH
		{ "You urgently need Feeding and Grooming. You are very unhappy.",	"You urgently need feeding and maybe a groom.",			"You urgently need Feeding." },   // hunger = LOW
		{ "You urgently need grooming and maybe a feeding.",				"You are Fairly content.",								"Ýou are very happy but could eat." },      // hunger = MEDIUM
		{ "You urgently need a grooming but is not hungry.",				"Your belly is full but you need a little grooming.",	"You are very happy and content with everything" }   // hunger = HIGH
	};
};
