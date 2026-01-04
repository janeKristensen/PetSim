#include "NeedsSystem.h"


void NeedsSystem::setModel(std::shared_ptr<Model> model) {

    mModel = model;
}

void NeedsSystem::update(float dt) {

    decayValues(dt);
    updateState();
}

void NeedsSystem::processItem(Item& item) {

    auto type = item.getTypeId();
    switch (type)
    {
        case ItemType::FOOD:
            mPet->setHungerValue(item.getValue());
            break;
        case ItemType::GROOM:
            mPet->setGroomValue(item.getValue());
            break;
        case ItemType::TOY:
            auto prev_happiness = mPet->getHappinessValue();
            mPet->setHappinessValue(item.getValue());
            if(prev_happiness < mPet->getHappinessValue()) mPet->increasedHappiness(true);
            break;
    }   
}

void NeedsSystem::decayValues(float dt) {

    mTimeTracker += dt;

    if (mTimeTracker >= DECAY_TIME) {

        mTimeTracker = 0;
        mPet->decayValues();
    }
}

void NeedsSystem::updateSystemPrompt() {

    auto newState = std::make_pair<int32_t, int32_t>((int32_t)mFeedState, (int32_t)mGroomState);

    if (newState != mPreviousState && mModel != nullptr) {

        std::string newPrompt = mStateTable[(int32_t)mFeedState][(int32_t)mGroomState];
        mPet->setStatus(newPrompt);
        mModel->addSystemPrompt(std::format("{} {}", mPet->getInitPrompt(), newPrompt));
    }
}

void NeedsSystem::updateState() {

    mPreviousState = { (int32_t)mFeedState, (int32_t)mGroomState };

    auto hunger = mPet->getHungerValue();
    auto groom = mPet->getGroomValue();  

    if (hunger >= 80) {

        mFeedState = State::HIGH;
    }
    else if (hunger < 80 && hunger >= 40) {

        mFeedState = State::MEDIUM;
    }
    else if (hunger < 40) {

        mFeedState = State::LOW;
    }

    if (groom >= 80) {

        mGroomState = State::HIGH;
    }
    else if (groom < 80 && groom >= 40) {

        mGroomState = State::MEDIUM;
    }
    else if (groom < 40) {

        mGroomState = State::LOW;
    }

    std::async(&NeedsSystem::updateSystemPrompt, this); 
}