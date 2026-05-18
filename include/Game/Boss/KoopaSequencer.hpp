#pragma once

#include "Game/System/NerveExecutor.hpp"
#include "stdbool.h"

class Koopa;
class JMapInfoIter;
class KoopaDemoPowerUp;
class KoopaDemoJumpToPlanet;

class KoopaSequencer : public NerveExecutor {
public:
    KoopaSequencer(const char*, s32);

    virtual ~KoopaSequencer();
    virtual void init(Koopa*, const JMapInfoIter&);
    virtual void update();
    virtual void calcAndSetBaseMtx();
    virtual void startBattleLevel2();
    virtual void startBattleLevel3();

    /* 0x8 */ Koopa* mKoopa;
    /* 0xC */ KoopaDemoPowerUp* mPowerUp;
    /* 0x10 */ u32 _10;
    /* 0x14 */ KoopaDemoJumpToPlanet* mJumpToPlanet;
    /* 0x18 */ u32 mLv;
    /* 0x1C */ u32 mVs;
    /* 0x20 */ bool mIsAngry;
};
