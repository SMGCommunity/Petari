#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class Coin;
class Spine;

class TripodBossCoin : public NameObj {
public:
    /// @brief Creates a new `TripodBossCoin`.
    /// @param pName A pointer to the null-terminated name of the object.
    TripodBossCoin(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void movement();

    void exeNonActive();
    void exeActive();
    void exeEnd();

    /* 0x0C */ Coin* mCoin;
    /* 0x10 */ Spine* mSpine;
    /* 0x14 */ TPos3f _14;
    /* 0x44 */ s32 mJointID;
};
