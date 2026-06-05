#pragma once

#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/SpinDriverPathDrawer.hpp"
#include "Game/MapObj/SpinDriverShootPath.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "revolution/mtx.h"


class AstroDomeDemoStarter : public LiveActor {
public:
    AstroDomeDemoStarter(const char*);
    virtual void init(const JMapInfoIter&);
    virtual void calcAndSetBaseMtx();
    virtual ~AstroDomeDemoStarter();

    void movePlayer();
    void startJumpOut();
    void exeSpinDriverAppear();
    void exeSpinDriverStart();
    void exeSpinDriverShoot();
    void exeJumpOut();
    void exeWhiteOut();

private:
    /* 0x8C */ SpinDriverShootPath* mSpinDriverShootPath;
    /* 0x90 */ SpinDriverPathDrawer* mSpinDriverPathDrawer;
    /* 0x94 */ TMtx34f _94;
    /* 0xC4 */ TMtx34f _C4;
    /* 0xF4 */ ActorCameraInfo* mCameraInfo;
};
