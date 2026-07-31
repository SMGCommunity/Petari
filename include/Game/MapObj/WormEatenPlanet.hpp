#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class GreenCaterpillarBig;
class LodCtrl;
class PartsModel;

class WormEatenPlanet : public LiveActor {
public:
    WormEatenPlanet(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();

    void exeFirstWait();
    void exeSecondWait();
    void exeThirdWait();
    void exeFourthWait();
    void initWormEatenHill();
    bool tryGenerateWormEatenHill();
    void startSecondDemo();
    void startThirdDemo();

    /* 0x8C */ GreenCaterpillarBig* mCaterpillar;
    /* 0x90 */ PartsModel** mWormEatenHill;
    /* 0x94 */ CollisionParts* mSecondHill;
    /* 0x98 */ CollisionParts* mThirdHill;
    /* 0x9C */ CollisionParts* mFourthHill;
    /* 0xA0 */ LodCtrl* mLodCtrl;
};
