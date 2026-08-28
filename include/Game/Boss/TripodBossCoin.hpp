#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class Coin;
class Spine;

class TripodBossCoin : public NameObj {
public:
    TripodBossCoin(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x14 */ virtual void movement();

    void exeNonActive();
    void exeActive();
    void exeEnd();

    /* 0x0C */ Coin* mCoin;
    /* 0x10 */ Spine* mSpine;
    /* 0x14 */ TPos3f _14;
    /* 0x44 */ s32 _44;
};
