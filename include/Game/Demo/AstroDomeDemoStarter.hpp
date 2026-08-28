#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class ActorCameraInfo;
class SpinDriverShootPath;
class SpinDriverPathDrawer;

class AstroDomeDemoStarter : public LiveActor {
public:
    AstroDomeDemoStarter(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x38 */ virtual MtxPtr getBaseMtx() const {
        return (MtxPtr)_94.mMtx;
    };
    /* 0x4C */ virtual void calcAndSetBaseMtx();

    void startJumpOut();
    void movePlayer();
    void exeSpinDriverAppear();
    void exeSpinDriverStart();
    void exeSpinDriverShoot();
    void exeJumpOut();
    void exeWhiteOut();

    SpinDriverShootPath* _8C;
    SpinDriverPathDrawer* _90;
    TPos3f _94;
    TPos3f _C4;

    ActorCameraInfo* _F4;
};
