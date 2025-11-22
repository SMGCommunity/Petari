#include "Game/Util/TriggerChecker.hpp"

TriggerChecker::TriggerChecker()
    : mPrevLevel(false),
      mCurrLevel(false) {}

void TriggerChecker::update(bool input) {
    mPrevLevel = mCurrLevel;
    mCurrLevel = input;
}

void TriggerChecker::setInput(bool input) {
    mPrevLevel = input;
    mCurrLevel = input;
}

bool TriggerChecker::getLevel() const {
    return mCurrLevel;
}

bool TriggerChecker::getOnTrigger() const {
    return !mPrevLevel && mCurrLevel;
}

bool TriggerChecker::getOffTrigger() const {
    return mPrevLevel && !mCurrLevel;
}
