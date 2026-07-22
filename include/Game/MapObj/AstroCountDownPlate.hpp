#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/J3DGraphBase/J3DTexture.hpp>

class AstroCountDownPlate : public LiveActor {
public:
    AstroCountDownPlate(const char*);

    virtual void init(const JMapInfoIter&);

    void exeWait();
    void exeRevival();
    void exeCountToZero();
    void initTextureAtNumLeftStar();
    void setNumLeftStar();
    void selectNrvWait();
    void setupStateWait();
    void startDemoStartCountDown();
    void startDemoLastBattle();

    /* 0x08C */ J3DTexMtx _8C;
    /* 0x120 */ J3DTexMtx _120;
    /* 0x1B4 */ bool _1B4;
};
