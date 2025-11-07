#include "Game/System/GameEventFlag.hpp"


GameEventFlagAccessor::GameEventFlagAccessor(const GameEventFlag *flag) {
    mFlag = flag;
}

const char* GameEventFlagAccessor::getName() const {
    return mFlag->mName;
}

bool GameEventFlagAccessor::isTypeGalaxyOpenStar() const {
    return mFlag->mType == 2;
}

bool GameEventFlagAccessor::isTypeSpecialStar() const {
    return mFlag->mType == 3;
}

bool GameEventFlagAccessor::isTypeStarPiece() const {
    return mFlag->mType == 8;
}

bool GameEventFlagAccessor::isTypeEventValueIsZero() const {
    return mFlag->mType == 9;
}

const char* GameEventFlagAccessor::getGalaxyName() const {
    return mFlag->mGalaxyName;
}

s32 GameEventFlagAccessor::getStarId() const {
    return mFlag->mStarID;
}

s32 GameEventFlagAccessor::getStarPieceIndex() const {
    return mFlag->mStarID;
}

const char* GameEventFlagAccessor::getGalaxyNameWithStarPiece() const {
    return mFlag->mGalaxyName;
}

s32 GameEventFlagAccessor::getNeedStarPieceNum() const {
    return mFlag->mStarPieceNum * 10;
}

const char* GameEventFlagAccessor::getEventValueName() const {
    return mFlag->mEventValueName;
}

const char* GameEventFlagAccessor::getRequirement() const {
    return mFlag->mRequirement;
}
