#include "NeedsSystem.h"




void NeedsSystem::update(float dt) {

    decayValues(dt);
    updateState();
    updateSystemPrompt();
}

void NeedsSystem::processItem(Item& item) {

    Food* food = dynamic_cast<Food*>(&item);
    if (food){

        mPet->setHungerValue(food->getValue());
    }
    else {

        GroomItem* groom = dynamic_cast<GroomItem*>(&item);
        if (groom) mPet->setGroomValue(groom->getValue());
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

    if (newState != mPreviousState) {

        if (mModel == nullptr) {
            mModel = mModelFuture.get();
            mModel->addSystemPrompt(mPet->getInitPrompt());
        }

        std::string newPromt = mStateTable[(int32_t)mFeedState][(int32_t)mGroomState];
        mModel->addSystemPrompt(newPromt);
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
}