#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/PartsModel.hpp"

class GreenCaterpillarBig;
class LodCtrl;

class WormEatenPlanet : public LiveActor {
public:
    WormEatenPlanet(const char*);

    virtual ~WormEatenPlanet();
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

    GreenCaterpillarBig* mCaterpillar;  // 0x8C
    PartsModel** mEatenHills;           // 0x90
    CollisionParts* mSecondHill;        // 0x94
    CollisionParts* mThirdHill;         // 0x98
    CollisionParts* mFourthHill;        // 0x9C
    LodCtrl* mLodCtrl;                  // 0xA0
};
